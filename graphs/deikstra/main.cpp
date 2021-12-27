#define NOMINMAX
#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include "..\\Algorithms_on_Graphs\common.h"

/// typedef std::map<std::string, float> weight_map;    /// название вершины -> вес ребра к этой вершине
/// typedef std::map<std::string, weight_map> graph;    /// название вершины -> набор вершин, в которые можно попасть из данной

float I = std::numeric_limits<float>::infinity();

float DLL_EXPORT shortest_length(const graph &graph, const std::string &src, const std::string &dst) 
{
    std::map<std::string, float> x;
	std::map<std::string, bool> vector_1;
	for (auto i : graph)
	{
		x.insert({ i.first,I });
		x.insert({ i.first,false });
	}
    
	x[src] = 0;
	for (auto i : graph)
	{
		std::string vector_2 = "last";
		weight_map  d;
		for (auto j : i.second)
			if (vector_1[j.first] == false && (vector_2 == "last" || x[j.first] < x[vector_2]))
			{
				vector_2 = j.first;
				d[vector_2] = j.second;
			}
		if (x[vector_2] == I)
			break;
		vector_1[vector_2] = true;
		for (auto k : d)
		{
			std::string to = k.first;
			float len = k.second;
			x[to] = std::min(x[vector_2] + len, x[to]);
		}
        
	}
	return x[dst];
}
