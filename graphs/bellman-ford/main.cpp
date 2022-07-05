#include <common.h>
#include <map>

float DLL_EXPORT shortest_length(const graph &graph, const std::string &src, const std::string &dst) {
	std::map<std::string, float> path;
	for (auto V : graph) {
		path.insert({ V.first, inf});
	}
	path[src] = 0;
	while (true) {
		bool any = false;
		for (auto V : graph) {
			for (auto j : V.second) {
				if (path[j.first] > path[V.first] + j.second) {
					path[j.first] = path[V.first] + j.second;
					any = true;
				}
			}
		}
		if (!any)
			break;
	}
	if (path[dst] == inf)
		return 0;
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
