#pragma once
#include <vector>
#include <unordered_map>
#include <utility>

struct Node {
    long long id;   // GraphML node id
    double lat;     // d4 (y)
    double lon;     // d5 (x)
};

struct EdgeInfo {
    int to;         // 목적지 노드 인덱스
    double length;  // 도로 길이
};

struct Graph {
    std::vector<Node> nodes;                       // index -> Node
    std::vector<std::vector<EdgeInfo>> adj;        // index -> edges
    std::unordered_map<long long, int> idToIndex;  // node id -> index

    int findNearestNode(double lat, double lon) const;
};
