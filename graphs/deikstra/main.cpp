#include "pch.h"
#include <map>
const int INF = 1000000000;

float shortest_length(const graph& graph, const std::string& src, const std::string& dst)
{

	std::map<std::string,float> a;
	std::map<std::string, bool> u;
	for (auto i : graph)
	{
		a.insert({ i.first,INF });
		a.insert({ i.first,false });
	}
	a[src] = 0;
	for (auto i : graph)
	{
		std::string v = "new";
		weight_map  d;
		for (auto j : i.second)
			if (u[j.first] == false && (v == "new" || a[j.first] < a[v]))
			{
				v = j.first;
				d[v] = j.second;
			}

		if (a[v] == INF)
			break;
		u[v] = true;

		for (auto k : d)
		{
			std::string to = k.first;
			float len = k.second;
			a[to] = min(a[v] + len, a[to]);
		}
	}
	return a[dst];
}
