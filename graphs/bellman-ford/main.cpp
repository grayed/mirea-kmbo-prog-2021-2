#include "pch.h"
#include "..\\GraphPathFinder\common.h"
#include <vector>

using namespace std;
struct edge
{
    std::string u, v;
    float w;
};

float DLL_EXPORT shortest_length(const graph& graph, const std::string& src, const std::string& dst)
{
    map<string, float> labels;
    vector<edge> tovisit;

    for (const auto& s : graph)
    {
        for (const auto& t : s.second)
        {
            tovisit.push_back({ s.first, t.first, t.second });
        }

        if (s.first == src)
            labels[s.first] = 0;
        else
            labels[s.first] = std::numeric_limits<float>::infinity();
    }

    for (int i = 0; i < graph.size() - 1; ++i)
    {
        for (int j = 0; j < tovisit.size(); ++j)
        {
            string from = tovisit[j].u;
            string to = tovisit[j].v;
            float w = tovisit[j].w;

            labels[to] = min(labels[to], labels[from] + w);
        }
    }
    return 0;
}
