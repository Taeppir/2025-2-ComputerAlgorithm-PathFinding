#pragma once
#include <string>
#include "Graph.h"

namespace GraphLoader {
    Graph loadFromGraphML(const std::string& filename);
}
