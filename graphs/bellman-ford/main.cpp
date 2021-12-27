#include <common.h>

float DLL_EXPORT shortest_length(const graph& graph, const std::string src, const std::string dst) {
    if (graph.find(src) == graph.end() || graph.find(dst) == graph.end())
        return -1; //если таких вершин нет

    std::map<std::string, float> path;
    for (const auto& g : graph)
        path.insert({ g.first, std::numeric_limits<float>::infinity() });

    for (int i = 0; i < graph.size() - 1; ++i) {
        for (const auto& g : graph)
            for (const auto& to : g.second)
                path[to.first] = min(path[to.first], path[g.first] + to.second);
    }

    if (path[dst] == std::numeric_limits<float>::infinity()) return -1;
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
