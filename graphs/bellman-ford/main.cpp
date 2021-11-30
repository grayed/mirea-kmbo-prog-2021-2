#include <iostream>
#include <fstream>
#include <typeinfo>
#include <string>
#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include<vector>

#include "..\\Project1\common.h"

using namespace std;


struct edge {
    string a, b;
    float w;
};


float DLL_EXPORT shortest_length(const graph& graph, const std::string& src, const std::string& dst) {
    /// TODO
    map<string, float> labels;//dist
    vector<edge> tovisit; // список ребер

    for (const auto& v : graph) {
        for (const auto& w : v.second) {
            tovisit.push_back({ v.first, w.first, w.second });
        }
        if (v.first == src)
            labels[v.first] = 0;
        else
            labels[v.first] = std::numeric_limits<float>::infinity();
    }

    for (size_t i = 0; i < graph.size() - 1; ++i) {
        for (size_t j = 0; j < tovisit.size(); ++j) {
            string v = tovisit[j].a;
            string to = tovisit[j].b;
            float w = tovisit[j].w;
            labels[to] = min(labels[to], labels[v] + w);
        }
    }


    return 0;
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
