#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include <common.h>

float DLL_EXPORT shortest_length(const graph &graph, const std::string &src, const std::string &dst) {
	std::map<std::string,float> path;
	std::map<std::string, bool> used;
	for (auto V : graph) {
		path.insert({ V.first, inf });
		path.insert({ V.first,false });
	}
	path[src] = 0;
	for (auto V : graph) {
		std::string v = "new";
		weight_map  d;
		for (auto k : V.second)
			if (used[k.first] == false && (v == "new" || path[k.first] < path[v])) {
				v = k.first;
				d[v] = k.second;
			}

		if (path[v] == inf)
			break;
		used[v] = true;

		for (auto k : d) {
			std::string to = k.first;
			float len = k.second;
			path[to] = min(path[v] + len, path[to]);
		}
	}
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
    return TRUE; // successful
}
