//#include <iostream>
//#include <limits>
//#include "GraphLoader.h"
//#include "Dijkstra.h"
//#include "MonteCarlo.h"
//
//void printPathNodeIds(const Graph& graph, const std::vector<int>& path) {
//    for (size_t i = 0; i < path.size(); ++i) {
//        long long id = graph.nodes[path[i]].id;
//        std::cout << id;
//        if (i + 1 < path.size()) std::cout << "-";
//    }
//    std::cout << std::endl;
//}
//
//int main() {
//
//    Graph graph = GraphLoader::loadFromGraphML("jongro.graphml");
//
//    if (graph.nodes.empty()) {
//        std::cerr << "Graph loading failed (no nodes)\n";
//        return 1;
//    }
//
//    std::cout << "Graph loaded. Node count = "
//        << graph.nodes.size() << std::endl;
//
//    double sLat, sLon, tLat, tLon;
//    std::cout << "Insert start position (lat lon): ";
//    std::cin >> sLat >> sLon;
//    std::cout << "Insert destination position (lat lon): ";
//    std::cin >> tLat >> tLon;
//
//    int s = graph.findNearestNode(sLat, sLon);
//    int t = graph.findNearestNode(tLat, tLon);
//
//    if (s < 0 || t < 0) {
//        std::cerr << "Cannot find nearest nodes\n";
//        return 1;
//    }
//
//    std::cout << "Start node index: " << s
//        << " (id=" << graph.nodes[s].id << ")\n";
//    std::cout << "Dest node index: " << t
//        << " (id=" << graph.nodes[t].id << ")\n";
//
//    // ---------------- Dijkstra ----------------
//    std::vector<int> dijkstraPath;
//    double dijkstraLen = runDijkstra(graph, s, t, dijkstraPath);
//
//    // ---------------- Monte Carlo -------------
//    std::vector<int> mcPath;
//    int maxSteps = 200;    // 한 시도에서 최대 step
//    int numTrials = 1000;  // 시도 횟수
//    double mcLen = runMonteCarlo(graph, s, t, maxSteps, numTrials, mcPath);
//
//    // ---------------- 결과 출력 --------------
//
//    const double INF = std::numeric_limits<double>::infinity();
//
//    std::cout << "\n=== Result ===\n";
//
//    if (mcLen == INF) {
//        std::cout << "Random Path Sampling shortest path length is : (no path found)\n";
//    }
//    else {
//        std::cout << "Random Path Sampling shortest path length is : "
//            << mcLen << " (total edge length)\n";
//        std::cout << "Random Path Sampling path is : ";
//        printPathNodeIds(graph, mcPath);
//    }
//
//    if (dijkstraLen == INF) {
//        std::cout << "Dijkstra algorithm path length is : (no path found)\n";
//    }
//    else {
//        std::cout << "Dijkstra algorithm path length is : "
//            << dijkstraLen << std::endl;
//        std::cout << "Dijkstra algorithm path is : ";
//        printPathNodeIds(graph, dijkstraPath);
//    }
//
//    return 0;
//}
