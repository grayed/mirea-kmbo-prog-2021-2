#include "../common.h"

#include <string>
#include <map>
#include <cfloat>

using namespace std;

extern "C" float shortest_length(graph& g, const std::string& src, 
        const std::string& dst)
{
    const float INF = FLT_MAX;
    map<string, float> dist;
    for (auto it = g.begin(); it != g.end(); it++) {
        dist[it->first] = INF;
    }
    dist[src] = 0;

    for (size_t k = 0; k < g.size() - 1; k++) {
        for (auto it = g.begin(); it != g.end(); it++) {
            string v = it->first;
            for (auto& e : it->second) {
                string u = e.first;
                float w = e.second;
                if (dist[v] < INF) {
                    dist[u] = min(dist[u], dist[v] + w);
                }
            }
        }
    }

    return dist[dst];
}

