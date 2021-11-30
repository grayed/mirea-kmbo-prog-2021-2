#include "../common.h"
#include "../../binary-heap.cpp"

#include <iostream>
#include <map>
#include <cfloat>

using namespace std;

extern "C" float shortest_length(graph& g, const std::string& src,
        const std::string& dst)
{
    const float INF = FLT_MAX;
    HeapOverArray<pair<float, string>> heap;
    map<string, float> dist;
    for (auto it = g.begin(); it != g.end(); it++) {
        dist[it->first] = INF;
    }
    dist[src] = 0;
    heap.addNode({0, src});
    while (!heap.empty()) {
        pair<float, string> p = heap.top();
        string v = p.second;
        float d = -p.first;
        heap.pop();
        if (dist[v] < d) {
            continue;
        }
        for (auto it = g[v].begin(); it != g[v].end(); it++) {
            if (dist[it->first] > d + it->second) {
                dist[it->first] = d + it->second;
                heap.addNode({-dist[it->first], it->first});
            }
        }
    }

    return dist[dst];
}

