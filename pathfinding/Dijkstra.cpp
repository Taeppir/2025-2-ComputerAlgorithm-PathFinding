#include "Dijkstra.h"
#include <queue>
#include <limits>

double runDijkstra(const Graph& graph, int startIdx, int goalIdx, std::vector<int>& outPath) {
    const int n = static_cast<int>(graph.nodes.size());
    const double INF = std::numeric_limits<double>::infinity();

    outPath.clear();
    if (startIdx < 0 || startIdx >= n || goalIdx < 0 || goalIdx >= n) {
        return INF;
    }

    std::vector<double> dist(n, INF);
    std::vector<int> prev(n, -1);

    // (거리, 노드 인덱스)
    typedef std::pair<double, int> State;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    dist[startIdx] = 0.0;
    pq.push(State(0.0, startIdx));

    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;   // outdated
        if (u == goalIdx) break;     // 목적지 도달

        const std::vector<EdgeInfo>& edges = graph.adj[u];
        for (size_t i = 0; i < edges.size(); ++i) {
            int v = edges[i].to;
            double nd = d + edges[i].length;
            if (nd < dist[v]) {
                dist[v] = nd;
                prev[v] = u;
                pq.push(State(nd, v));
            }
        }
    }

    if (dist[goalIdx] == INF) {
        // 경로 없음
        return INF;
    }

    // 경로 복원 (goal -> ... -> start)
    std::vector<int> revPath;
    for (int cur = goalIdx; cur != -1; cur = prev[cur]) {
        revPath.push_back(cur);
        if (cur == startIdx) break;
    }

    if (revPath.back() != startIdx) {
        return INF;
    }

    // 뒤집어서 start -> ... -> goal
    outPath.assign(revPath.rbegin(), revPath.rend());
    return dist[goalIdx];
}
