// pch.cpp: файл исходного кода, соответствующий предварительно скомпилированному заголовочному файлу

#include "pch.h"
#include <map>
const int INF = 1000000000;

float shortest_length(const graph& graph, const std::string& src, const std::string& dst)
{

	std::map<std::string, float> a;
	for (auto i : graph)
	{
		a.insert({ i.first,INF });
	}
	a[src] = 0;
	for (;;)
	{
		bool any = false;
		for (auto i : graph)
		{
			for (auto j : i.second)
			{
				if (a[j.first] > a[i.first] + j.second)
				{
					a[j.first] = a[i.first] + j.second;
					any = true;
				}
			}
		}
		if (!any)
			break;
	}
	if (a[dst] == INF)
		return 0;
	return a[dst];
}
