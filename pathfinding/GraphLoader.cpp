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
        bool oneway = false;

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
            else if (k == "d13") {      // oneway
                std::string v = text;
                oneway = (v == "True" || v == "true" || v == "1");
            }
        }

        if (!hasLength) length = 1.0;

        // 방향: src -> tgt
        graph.adj[srcIdx].push_back({ tgtIdx, length });

        // oneway가 아니라면 양방향으로 추가
        if (!oneway) {
            graph.adj[tgtIdx].push_back({ srcIdx, length });
        }
    }

    std::cout << "Loaded graph: " << graph.nodes.size()
        << " nodes" << std::endl;
    return graph;
}
