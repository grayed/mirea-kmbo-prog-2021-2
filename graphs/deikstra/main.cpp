#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include <common.h>

/// typedef std::map<std::string, float> weight_map;    /// название вершины -> вес ребра к этой вершине
/// typedef std::map<std::string, weight_map> graph;    /// название вершины -> набор вершин, в которые можно попасть из данной

float DLL_EXPORT shortest_length(const graph &graph, const std::string &src, const std::string &dst) {
    auto search = graph.find(src);	//проверяем наличие вершины в графе 
	auto search2 = graph.find(dst);
	if (search == graph.end() || search2 == graph.end())
		return -1;

	std::map<std::string, float> d;
	std::map<std::string, bool> used;
	for (const auto& g : graph) {
		used.insert({ g.first, false });
		d.insert({ g.first, INF });
	}
	d[src] = 0;
	std::string temp;
	for (const auto& g : graph) {
		float min = INF;
		for (const auto& w : g.second)
			if (!used[w.first] && d[w.first] < min) {
				min = d[w.first];
				temp = w.first;
			}
		used[temp] = true;

		for (const auto& w : g.second)
			if (w.second != -1 && !used[w.first] && d[temp] + w.second < d[w.first])
				d[w.first] = d[temp] + w.second;
	}
	if (d[dst] == INF)
		return -1;
	return d[dst];
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
