//#include <iostream>
//#include <string>
//#include "Graph.h"
//#include "tinyxml2.h"
//
//using namespace tinyxml2;
//
//// ========================================
//// 방법 1: 기존 코드
//// (oneway 처리 - 중복 발생)
//// ========================================
//Graph loadOriginal(const std::string& filename) {
//    Graph graph;
//    XMLDocument doc;
//    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) return graph;
//
//    XMLElement* graphElem = doc.FirstChildElement("graphml")->FirstChildElement("graph");
//
//    // 노드 읽기
//    for (XMLElement* nodeElem = graphElem->FirstChildElement("node");
//        nodeElem; nodeElem = nodeElem->NextSiblingElement("node")) {
//
//        const char* idStr = nodeElem->Attribute("id");
//        if (!idStr) continue;
//
//        long long nodeId = std::stoll(idStr);
//        double lat = 0.0, lon = 0.0;
//        bool hasLat = false, hasLon = false;
//
//        for (XMLElement* dataElem = nodeElem->FirstChildElement("data");
//            dataElem; dataElem = dataElem->NextSiblingElement("data")) {
//
//            const char* key = dataElem->Attribute("key");
//            const char* text = dataElem->GetText();
//            if (!key || !text) continue;
//
//            std::string k = key;
//            if (k == "d4") { lat = std::stod(text); hasLat = true; }
//            else if (k == "d5") { lon = std::stod(text); hasLon = true; }
//        }
//
//        if (hasLat && hasLon) {
//            int idx = (int)graph.nodes.size();
//            graph.nodes.push_back({ nodeId, lat, lon });
//            graph.idToIndex[nodeId] = idx;
//        }
//    }
//
//    graph.adj.resize(graph.nodes.size());
//
//    // 간선 읽기 (기존 방식)
//    for (XMLElement* edgeElem = graphElem->FirstChildElement("edge");
//        edgeElem; edgeElem = edgeElem->NextSiblingElement("edge")) {
//
//        const char* srcStr = edgeElem->Attribute("source");
//        const char* tgtStr = edgeElem->Attribute("target");
//        if (!srcStr || !tgtStr) continue;
//
//        long long srcId = std::stoll(srcStr);
//        long long tgtId = std::stoll(tgtStr);
//
//        auto itSrc = graph.idToIndex.find(srcId);
//        auto itTgt = graph.idToIndex.find(tgtId);
//        if (itSrc == graph.idToIndex.end() || itTgt == graph.idToIndex.end()) continue;
//
//        int srcIdx = itSrc->second;
//        int tgtIdx = itTgt->second;
//
//        double length = 1.0;
//        bool oneway = false;
//
//        for (XMLElement* dataElem = edgeElem->FirstChildElement("data");
//            dataElem; dataElem = dataElem->NextSiblingElement("data")) {
//
//            const char* key = dataElem->Attribute("key");
//            const char* text = dataElem->GetText();
//            if (!key || !text) continue;
//
//            std::string k = key;
//            if (k == "d15") { length = std::stod(text); }
//            else if (k == "d13") {
//                std::string v = text;
//                oneway = (v == "True" || v == "true" || v == "1");
//            }
//        }
//
//        // 원본 로직: oneway 처리
//        graph.adj[srcIdx].push_back({ tgtIdx, length });
//        if (!oneway) {
//            graph.adj[tgtIdx].push_back({ srcIdx, length });  // 중복 발생하는 부분
//        }
//    }
//
//    return graph;
//}
//
//// ========================================
//// 방법 2: 수정된 코드
//// (oneway 처리 제거)
//// ========================================
//Graph loadCorrected(const std::string& filename) {
//    Graph graph;
//    XMLDocument doc;
//    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) return graph;
//
//    XMLElement* graphElem = doc.FirstChildElement("graphml")->FirstChildElement("graph");
//
//    // 노드 읽기
//    for (XMLElement* nodeElem = graphElem->FirstChildElement("node");
//        nodeElem; nodeElem = nodeElem->NextSiblingElement("node")) {
//
//        const char* idStr = nodeElem->Attribute("id");
//        if (!idStr) continue;
//
//        long long nodeId = std::stoll(idStr);
//        double lat = 0.0, lon = 0.0;
//        bool hasLat = false, hasLon = false;
//
//        for (XMLElement* dataElem = nodeElem->FirstChildElement("data");
//            dataElem; dataElem = dataElem->NextSiblingElement("data")) {
//
//            const char* key = dataElem->Attribute("key");
//            const char* text = dataElem->GetText();
//            if (!key || !text) continue;
//
//            std::string k = key;
//            if (k == "d4") { lat = std::stod(text); hasLat = true; }
//            else if (k == "d5") { lon = std::stod(text); hasLon = true; }
//        }
//
//        if (hasLat && hasLon) {
//            int idx = (int)graph.nodes.size();
//            graph.nodes.push_back({ nodeId, lat, lon });
//            graph.idToIndex[nodeId] = idx;
//        }
//    }
//
//    graph.adj.resize(graph.nodes.size());
//
//    // 간선 읽 
//    for (XMLElement* edgeElem = graphElem->FirstChildElement("edge");
//        edgeElem; edgeElem = edgeElem->NextSiblingElement("edge")) {
//
//        const char* srcStr = edgeElem->Attribute("source");
//        const char* tgtStr = edgeElem->Attribute("target");
//        if (!srcStr || !tgtStr) continue;
//
//        long long srcId = std::stoll(srcStr);
//        long long tgtId = std::stoll(tgtStr);
//
//        auto itSrc = graph.idToIndex.find(srcId);
//        auto itTgt = graph.idToIndex.find(tgtId);
//        if (itSrc == graph.idToIndex.end() || itTgt == graph.idToIndex.end()) continue;
//
//        int srcIdx = itSrc->second;
//        int tgtIdx = itTgt->second;
//
//        double length = 1.0;
//
//        for (XMLElement* dataElem = edgeElem->FirstChildElement("data");
//            dataElem; dataElem = dataElem->NextSiblingElement("data")) {
//
//            const char* key = dataElem->Attribute("key");
//            const char* text = dataElem->GetText();
//            if (!key || !text) continue;
//
//            std::string k = key;
//            if (k == "d15") { length = std::stod(text); }
//        }
//
//		// 수정된 로직: oneway 처리 제거
//        graph.adj[srcIdx].push_back({ tgtIdx, length });
//    }
//
//    return graph;
//}
//
//int main() {
//    std::string filename = "jongro.graphml";
//
//    std::cout << "========================================\n";
//    std::cout << "기존 코드 vs 수정 코드 비교\n";
//    std::cout << "========================================\n\n";
//
//    // 1. 원본 코드로 로딩
//    std::cout << "[1] 원본 코드 (oneway 처리 포함)\n";
//    Graph origGraph = loadOriginal(filename);
//    int origEdges = 0;
//    for (const auto& adj : origGraph.adj) origEdges += adj.size();
//    std::cout << "    노드: " << origGraph.nodes.size() << "개\n";
//    std::cout << "    간선: " << origEdges << "개\n\n";
//
//    // 2. 수정 코드로 로딩
//    std::cout << "[2] 수정 코드 (oneway 처리 제거)\n";
//    Graph corrGraph = loadCorrected(filename);
//    int corrEdges = 0;
//    for (const auto& adj : corrGraph.adj) corrEdges += adj.size();
//    std::cout << "    노드: " << corrGraph.nodes.size() << "개\n";
//    std::cout << "    간선: " << corrEdges << "개\n\n";
//
//    // 3. GraphML 실제 간선 수
//    XMLDocument doc;
//    doc.LoadFile(filename.c_str());
//    XMLElement* graphElem = doc.FirstChildElement("graphml")->FirstChildElement("graph");
//    int xmlEdgeCount = 0;
//    for (XMLElement* edgeElem = graphElem->FirstChildElement("edge");
//        edgeElem; edgeElem = edgeElem->NextSiblingElement("edge")) {
//        xmlEdgeCount++;
//    }
//
//    std::cout << "========================================\n";
//    std::cout << "비교 결과\n";
//    std::cout << "========================================\n\n";
//
//    std::cout << "GraphML 파일 내 실제 간선: " << xmlEdgeCount << "개\n\n";
//
//    std::cout << "원본 코드:\n";
//    std::cout << "  생성 간선: " << origEdges << "개\n";
//    std::cout << "  비율: " << (double)origEdges / xmlEdgeCount << "배\n";
//    std::cout << "  중복: " << (origEdges - xmlEdgeCount) << "개\n";
//    std::cout << "  결과: 중복 발생\n\n";
//
//    std::cout << "수정 코드:\n";
//    std::cout << "  생성 간선: " << corrEdges << "개\n";
//    std::cout << "  비율: " << (double)corrEdges / xmlEdgeCount << "배\n";
//    std::cout << "  중복: " << (corrEdges - xmlEdgeCount) << "개\n";
//    std::cout << "  결과: 정확히 일치\n\n";
//
//    // 4. 구체적 예시
//    std::cout << "========================================\n";
//    std::cout << "구체적 예시\n";
//    std::cout << "========================================\n\n";
//
//    for (int i = 0; i < 100 && i < (int)origGraph.nodes.size(); i++) {
//        if (origGraph.adj[i].size() > corrGraph.adj[i].size()) {
//            std::cout << "노드 " << origGraph.nodes[i].id << " 의 이웃:\n\n";
//
//            std::cout << "원본 (" << origGraph.adj[i].size() << "개):\n";
//            for (size_t j = 0; j < std::min((size_t)10, origGraph.adj[i].size()); j++) {
//                std::cout << "  -> " << origGraph.nodes[origGraph.adj[i][j].to].id;
//                if (j >= 3) std::cout << "  (중복)";
//                std::cout << "\n";
//            }
//
//            std::cout << "\n수정 (" << corrGraph.adj[i].size() << "개):\n";
//            for (size_t j = 0; j < corrGraph.adj[i].size(); j++) {
//                std::cout << "  -> " << corrGraph.nodes[corrGraph.adj[i][j].to].id << "\n";
//            }
//
//            std::cout << "\n중복 제거: " << (origGraph.adj[i].size() - corrGraph.adj[i].size()) << "개\n";
//            break;
//        }
//    }
//    return 0;
//}