#include "Graph.h"
#include <limits>
#include <cmath>

int Graph::findNearestNode(double lat, double lon) const {
    int bestIdx = -1;
    double bestDist2 = std::numeric_limits<double>::infinity();

    for (int i = 0; i < (int)nodes.size(); ++i) {
        double dlat = nodes[i].lat - lat;
        double dlon = nodes[i].lon - lon;
        double dist2 = dlat * dlat + dlon * dlon;
        if (dist2 < bestDist2) {
            bestDist2 = dist2;
            bestIdx = i;
        }
    }
    return bestIdx;
}
