#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include <common.h>

/// typedef std::map<std::string, float> weight_map;    /// название вершины -> вес ребра к этой вершине
/// typedef std::map<std::string, weight_map> graph;    /// название вершины -> набор вершин, в которые можно попасть из данной

float DLL_EXPORT shortest_length(const graph& graph, const std::string src, const std::string dst) {
    if (graph.find(src) == graph.end() || graph.find(dst) == graph.end())
        return -1;

    std::map<std::string, bool> used;
    std::map<std::string, float> path;
    for (const auto& g : graph) {
        used.insert({ g.first, false });
        path.insert({ g.first, 1e9 });
    }

    path[src] = 0;
    std::string vert;
    for (const auto& g : graph) {
        float m = 1e9;
        for (const auto& to : g.second)
            if (!used[to.first] && path[to.first] < path[vert])
                vert = to.first;
        used[vert] = true;

        for (const auto& to : g.second)
            if (!used[to.first])
                path[to.first] = min(path[to.first], path[vert] + to.second);
    }

    if (path[dst] == 1e9) return -1;
    return path[dst];
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
