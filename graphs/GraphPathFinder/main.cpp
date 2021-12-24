#include <iostream>
#include <map>
#include <string>

#include "common.h"

using namespace std;

graph build_graph(std::istream &is) 
{
    std::string src = "";
    std::string dst = "";
    float weight = 0;

    graph graph;

    while (true)
    {
        is >> src >> dst >> weight;
        if (src == "")
            break;
        
        graph.insert(std::pair<std::string, weight_map>(dst, weight_map()));
        std::pair<graph::iterator, bool> insres = graph.insert(std::pair<std::string, weight_map>(dst, weight_map()));
        insres.first->second.insert(std::pair<std::string, float>(dst, weight));
    }
    return graph;
}

int main(int argc, char **argv)
{
    if (argc < 2) 
    {
        cerr << "Need at least 2 points to search" << endl;
        return 1;
    }

    auto graph = build_graph(cin);

    string src_name(argv[1]);
    string dst_name(argv[2]);

    auto src_it = graph.find(src_name);
    auto dst_it = graph.find(dst_name);

    if (src_it == graph.end()) 
    {
        cerr << "Unknown source: " << src_name << endl;
        return 1;
    }
    if (dst_it == graph.end()) 
    {
        cerr << "Unknown destination: " << dst_name << endl;
        return 1;
    }
    auto result = shortest_length(graph, src_it->first, dst_it->first);

    cout << result << endl;
    return 0;
}
