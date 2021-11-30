#include <common.h>

float DLL_EXPORT shortest_length(const graph &graph, const std::string &src, const std::string &dst) {
    auto search = graph.find(src);	//проверяем есть ли такие вершины в графе
	auto search2 = graph.find(dst);
	if (search == graph.end() || search2 == graph.end())
		return -1;

	std::map<std::string, float> d;
	for (const auto& g : graph) {
		d.insert({ g.first, INF });
	}
	for (int i = 0; i < graph.size() - 1; ++i)
	{
		for (const auto& g : graph)
			for (const auto& w : g.second)
			{
				if (d[w.first] > d[g.first] + w.second)
					d[w.first] = d[g.first] + w.second;
			}
	}
	if (d[dst] == INF) return -1;
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
