#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include <common.h>

/// typedef std::map<std::string, float> weight_map;    /// название вершины -> вес ребра к этой вершине
/// typedef std::map<std::string, weight_map> graph;    /// название вершины -> набор вершин, в которые можно попасть из данной

float DLL_EXPORT shortest_length(const graph &graph, const std::string &src, const std::string &dst)
{
    auto search_1 = graph.find(src);
	auto search_2 = graph.find(dst);

	if (search_1 == graph.end() || search_2 == graph.end())
		return -1;

	std::map<std::string, float> v;
	std::map<std::string, bool> u;
	for (const auto& g : graph) {
		u.insert({ g.first, false });
		v.insert({ g.first, INF });
	}

	v[src] = 0;
	std::string temp;
	for (const auto& g : graph) 
	{
		float min = INF;
		for (const auto& w : g.second)
			if (!u[w.first] && v[w.first] < min) 
			{
				min = v[w.first];
				temp = w.first;
			}
		u[temp] = true;

		for (const auto& w : g.second)
			if (w.second != -1 && !u[w.first] && v[temp] + w.second < v[w.first])
				v[w.first] = v[temp] + w.second;
	}

	if (v[dst] == INF)
		return -1;
	return v[dst];
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
