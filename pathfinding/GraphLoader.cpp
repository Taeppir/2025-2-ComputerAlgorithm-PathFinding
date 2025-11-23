#include "GraphLoader.h"
#include "tinyxml2.h"
#include <iostream>
#include <string>

using namespace tinyxml2;

Graph GraphLoader::loadFromGraphML(const std::string& filename) {
    Graph graph;

    XMLDocument doc;
    XMLError err = doc.LoadFile(filename.c_str());
    if (err != XML_SUCCESS) {
        std::cerr << "Failed to load file: " << filename
            << " error: " << doc.ErrorStr() << std::endl;
        return graph;
    }

    XMLElement* root = doc.FirstChildElement("graphml");
    if (!root) {
        std::cerr << "No <graphml> root element\n";
        return graph;
    }

    XMLElement* graphElem = root->FirstChildElement("graph");
    if (!graphElem) {
        std::cerr << "No <graph> element\n";
        return graph;
    }

    // 1) node 읽기
    for (XMLElement* nodeElem = graphElem->FirstChildElement("node");
        nodeElem;
        nodeElem = nodeElem->NextSiblingElement("node")) {

        const char* idStr = nodeElem->Attribute("id");
        if (!idStr) continue;

        long long nodeId = std::stoll(idStr);
        double lat = 0.0;
        double lon = 0.0;
        bool hasLat = false, hasLon = false;

        for (XMLElement* dataElem = nodeElem->FirstChildElement("data");
            dataElem;
            dataElem = dataElem->NextSiblingElement("data")) {

            const char* key = dataElem->Attribute("key");
            const char* text = dataElem->GetText();
            if (!key || !text) continue;

            std::string k = key;
            if (k == "d4") {          // y = latitude
                lat = std::stod(text);
                hasLat = true;
            }
            else if (k == "d5") {   // x = longitude
                lon = std::stod(text);
                hasLon = true;
            }
        }

        if (hasLat && hasLon) {
            int idx = (int)graph.nodes.size();
            graph.nodes.push_back({ nodeId, lat, lon });
            graph.idToIndex[nodeId] = idx;
        }
    }

    graph.adj.resize(graph.nodes.size());

    // 2) edge 읽기
    for (XMLElement* edgeElem = graphElem->FirstChildElement("edge");
        edgeElem;
        edgeElem = edgeElem->NextSiblingElement("edge")) {

        const char* srcStr = edgeElem->Attribute("source");
        const char* tgtStr = edgeElem->Attribute("target");
        if (!srcStr || !tgtStr) continue;

        long long srcId = std::stoll(srcStr);
        long long tgtId = std::stoll(tgtStr);

        auto itSrc = graph.idToIndex.find(srcId);
        auto itTgt = graph.idToIndex.find(tgtId);
        if (itSrc == graph.idToIndex.end() || itTgt == graph.idToIndex.end()) {
            continue; // node 정보 없는 edge는 버림
        }

        int srcIdx = itSrc->second;
        int tgtIdx = itTgt->second;

        double length = 1.0;
        bool hasLength = false;

        for (XMLElement* dataElem = edgeElem->FirstChildElement("data");
            dataElem;
            dataElem = dataElem->NextSiblingElement("data")) {

            const char* key = dataElem->Attribute("key");
            const char* text = dataElem->GetText();
            if (!key || !text) continue;

            std::string k = key;
            if (k == "d15") {             // length
                length = std::stod(text);
                hasLength = true;
            }
        }

        if (!hasLength) length = 1.0;

        // --------------------------------------------------------------
        // 간선 처리 로직 수정 (김태희, 2025.11.23)
        // --------------------------------------------------------------
        // readme.txt 에도 설명 추가해두었습니다.
        // OSMnx가 생성한 GraphML은 이미 directed graph 형식입니다.
        // 
        // - 양방향 도로는 2개의 directed edge로 저장됩니다.
        //   예: 도로 A ↔ B 이면, GraphML에 A→B, B→A 2개 간선으로 저장되고 있습니다.
        // 
        // - 일방통행은 1개의 directed edge로 저장됩니다.
        //   예: A→B만 가능 → GraphML에 A→B 1개 간선만 저장
        //
        // 따라서 각 간선을 source → target 방향으로만 추가하면 됩니다.
        // oneway(d13), reversed(d14) 속성은 메타데이터이며, 실제 그래프 구조는
        // 이미 GraphML에 올바르게 반영되어 있습니다.
        //
        // 기존 코드 문제점:
        //   - oneway=False인 간선마다 양방향 처리를 하여 중복 발생
        //   - GraphML 간선 18,189개 → 생성된 간선 35,429개 (약 2배 중복)
        //
        // 수정 후:
        //   - 각 간선을 있는 그대로만 추가
        //   - GraphML 간선 18,189개 → 생성된 간선 18,189개 (정확히 일치)
        //   - test_graphml_loading.cpp 실행하면 결과 확인할 수 있습니다.
        // --------------------------------------------------------------
        
        graph.adj[srcIdx].push_back({ tgtIdx, length });
    }

    std::cout << "Loaded graph: " << graph.nodes.size()
        << " nodes" << std::endl;
    return graph;
}
