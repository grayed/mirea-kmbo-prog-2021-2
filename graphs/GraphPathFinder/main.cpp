#include <iostream>
#include <map>
#include <string>

#include "common.h"

using namespace std;

/// вершина -> { вершина -> вес }


graph build_graph(std::istream &is) 
{
    graph g;
    int ver;    //maybe long int?
    is >> ver;
    for (auto i = 0; i < ver; i++) 
    {
        string st1, st2;
        float k;
        is >> st1 >> st2 >> k;
        g[st1][st2] = k;    //2-dim array
    }
    return g;
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
    }
    if (dst_it == graph.end()) 
    {
        cerr << "Unknown destination: " << dst_name << endl;
    }

    auto result = shortest_length(graph, src_it->first, dst_it->first);
    cout << result << endl;
    return 0;
}
