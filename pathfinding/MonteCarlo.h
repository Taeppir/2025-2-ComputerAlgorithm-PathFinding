#pragma once
#include <vector>
#include "Graph.h"

// Monte Carlo Random Path Sampling
// - maxSteps : 한 번의 시도에서 최대 몇 step까지 갈지 (예: 200)
// - numTrials: 전체 시도 횟수 (예: 1000)
// - outPath  : 발견된 최단 랜덤 경로 (노드 인덱스 리스트)
// 반환값 : 발견된 경로 길이. 한 번도 목적지에 도달 못하면 INF 리턴.
double runMonteCarlo(const Graph& graph,
    int startIdx,
    int goalIdx,
    int maxSteps,
    int numTrials,
    std::vector<int>& outPath);
