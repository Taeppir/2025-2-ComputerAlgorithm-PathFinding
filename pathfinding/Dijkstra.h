#pragma once
#include <vector>
#include "Graph.h"

// Dijkstra 최단거리 알고리즘
// - graph : 너가 만든 Graph
// - startIdx, goalIdx : 노드 인덱스 (findNearestNode 결과)
// - outPath : 최단 경로 (노드 인덱스들의 나열)
// 반환값 : 최단 경로 길이 (double). 경로 없으면 무한대(INF) 리턴.
double runDijkstra(const Graph& graph, int startIdx, int goalIdx, std::vector<int>& outPath);
