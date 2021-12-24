#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include <vector>
#include "..\\Algorithms_on_Graphs\common.h"

struct Edges
{
    std::string e1, e2;
    float weight;
};

float DLL_EXPORT shortest_length(const graph &graph, const std::string &src, const std::string &dst) 
{
    std::map<std::string, float> labs;
    std::vector<Edges> tovisit;
    for (const auto& pass : graph)
    {
        for (const auto& t : pass.second)
        {
            tovisit.push_back({ pass.first, t.first, t.second });   //rec
        }
        if (pass.first == src)
            labs[pass.first] = 0;
        else
            labs[pass.first] = std::numeric_limits<float>::infinity();  //inf labels
    }
    for (auto i = 0; i < graph.size() - 1; i++)
    {
        for (auto j = 0; j < tovisit.size(); i++)
        {
            std::string start = tovisit[j].e1;
            std::string finish = tovisit[j].e2;
            float weight = tovisit[j].weight;
            labs[finish] = min(labs[finish], labs[start] + weight);
        }
    }
    return 0;
}


