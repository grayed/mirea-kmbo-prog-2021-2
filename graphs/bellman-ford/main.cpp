#define NOMINMAX

#include <algorithm>
#include <limits>
#include <map>
#include <queue>

#include "..\\graphes\common.h"

/// typedef std::map<std::string, float> weight_map;    /// название вершины -> вес ребра к этой вершине
/// typedef std::map<std::string, weight_map> graph;    /// название вершины -> набор вершин, в которые можно попасть из данной

float DLL_EXPORT shortest_length(const graph& graph, const std::string& src, const std::string& dst)
{
    std::map<std::string, float> labels; /// вес вершин
    std::queue<std::string> tovisit; /// вершины, которые нужно посетить

    /// Проставить метки
    for (const auto& kv : graph)
    {
        if (kv.first == src)
            labels[kv.first] = 0;
        else
            labels[kv.first] = std::numeric_limits<float>::infinity();
    }

    /// пока считаем что отрицательного цикла в гарфе нет
    /// я написал bfs на стероидах тк не понял как писать по другому
    /// мы берем первую вершину в очереди и обновляем веса для ее соседей
    /// если получающийся вес меньше имеющегося то обновляем
    /// если вес соседа обновили то включаем его в очередь
    /// и так до тех пор пока очередь не кончится
    tovisit.push(src);
    while (tovisit.size() > 0)
    {
        auto vi = tovisit.front();

        for (const auto& neighbour : graph.at(vi))
        {
            /// neighbor.first - имя соседней вершины
            /// neighbor.second - расстояние до соседней вершины
            if (labels[vi] + neighbour.second < labels[neighbour.first])
            {
                labels[neighbour.first] = std::min(labels[vi] + neighbour.second, labels[neighbour.first]);
                tovisit.push(neighbour.first);
            }
        }
        tovisit.pop();
    }
    return labels.at(dst);
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
