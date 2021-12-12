#include "pch.h"
#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include "..\\GraphPathFinder\common.h"

/// typedef std::map<std::string, float> weight_map;    /// название вершины -> вес ребра к этой вершине
/// typedef std::map<std::string, weight_map> graph;    /// название вершины -> набор вершин, в которые можно попасть из данной

float DLL_EXPORT shortest_length(const graph& graph, const std::string& src, const std::string& dst) 
{
    if (graph.find(src) == graph.end() || graph.find(dst) == graph.end())
        return -1;

    std::map<std::string, bool> used;
    std::map<std::string, float> path;
    float inf = std::numeric_limits<float>::infinity();

    for (const auto& g : graph)
    {
        used.insert({ g.first, false });
        path.insert({ g.first, inf });
    }

    path[src] = 0;
    std::string v;
    for (const auto& g : graph)
    {
        for (const auto& to : g.second)
            if (used[to.first] == 0 && path[to.first] < path[v])
                v = to.first;
        used[v] = true;

        for (const auto& to : g.second)
            if (used[to.first] == 0)
                path[to.first] = min(path[to.first], path[v] + to.second);
    }

    if (path[dst] == inf)
        return -1;
    return path[dst];
}
