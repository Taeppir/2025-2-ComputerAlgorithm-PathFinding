#include "MonteCarlo.h"
#include <random>
#include <chrono>
#include <limits>

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

    double bestLen = INF;
    std::vector<int> bestPath;

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

        if (!path.empty() && path.back() == goalIdx) {
            // 목적지에 도달한 경로 중에서 최소 길이 선택
            if (totalLen < bestLen) {
                bestLen = totalLen;
                bestPath = path;
            }
        }
    }

    if (bestLen == INF) {
        // 한 번도 목적지까지 도달 못함
        return INF;
    }

    outPath = bestPath;
    return bestLen;
}
