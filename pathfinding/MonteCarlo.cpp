#include "MonteCarlo.h"
#include <random>
#include <chrono>
#include <limits>
#include <iostream>

double runMonteCarlo(const Graph& graph,
    int startIdx,
    int goalIdx,
    int maxSteps,
    int numTrials,
    std::vector<int>& outPath) {
    const int n = static_cast<int>(graph.nodes.size());
    const double INF = std::numeric_limits<double>::infinity();
    outPath.clear();

    if (startIdx < 0 || startIdx >= n || goalIdx < 0 || goalIdx >= n) {
        return INF;
    }

    // 랜덤 엔진 초기화
    std::mt19937 rng(static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	
    int bestSteps = INT_MAX;  // 스텝 수가 최소인 경로 찾기
    double bestLen = INF;
    std::vector<int> bestPath;
    int successCount = 0;

    for (int trial = 0; trial < numTrials; ++trial) {
        int current = startIdx;
        double totalLen = 0.0;
        std::vector<int> path;
        path.push_back(current);

        for (int step = 0; step < maxSteps; ++step) {
            if (current == goalIdx) {
                // 목적지 도달
                break;
            }

            const auto& neighbors = graph.adj[current];
            if (neighbors.empty()) {
                // 더 이상 갈 곳 없음
                break;
            }

            // 이웃들 중 랜덤 하나 선택
            std::uniform_int_distribution<int> dist(0, static_cast<int>(neighbors.size()) - 1);
            int idx = dist(rng);
            const auto& edge = neighbors[idx];

            current = edge.to;
            totalLen += edge.length;
            path.push_back(current);
        }

        // 목적지 도달 시
        if (!path.empty() && path.back() == goalIdx) {
            successCount++;
            int numSteps = path.size() - 1;

            // step 수가 최소인 경로 선택
            if (numSteps < bestSteps) {
                bestSteps = numSteps;
                bestLen = totalLen;
                bestPath = path;
            }
        }
    }
    // 디버깅 정보 (이건 추가해도 OK)
    std::cout << "[Monte Carlo] Trials: " << numTrials
        << ", Success: " << successCount;
    if (successCount > 0) {
        std::cout << " (" << (successCount * 100.0 / numTrials) << "%)";
    }
    std::cout << std::endl;

    if (bestSteps == INT_MAX) return INF;

    outPath = bestPath;
    return bestLen;
}
