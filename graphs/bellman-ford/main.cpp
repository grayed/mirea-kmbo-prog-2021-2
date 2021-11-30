#include "pch.h"
#include "..\GraphShortPath\common.h"
#include <map>
#define INF 1e6

float DLL_EXPORT shortest_length(const graph& graph, const std::string& src, const std::string& dst) {
	auto search = graph.find(src);	//проверяем наличие вершины в графе 
	auto search2 = graph.find(dst);
	if (search == graph.end() || search2 == graph.end())
		return -1;

	std::map<std::string, bool> d;
	for (const auto& g : graph) {
		d.insert({ g.first, INF });
	}
	for (auto i = 0; i < graph.size() - 1; ++i) {
		for (const auto& g : graph)
			for (const auto& w : g.second)
			{
				if (d[w.first] > d[g.first] + w.second)
					d[w.first] = d[g.first] + w.second;
			}
	}
	//return 0;
	if (d[dst] == INF)
		return -1;
	return d[dst];
}
