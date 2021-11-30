#include <iostream>
#include <map>
#include <string>

#include "common.h"

using namespace std;

graph build_graph(std::istream& is) {
    graph g;
    int m;
    is >> m;
    for (int i = 0; i < m; i++) {
        string v, w;
        float k;
        is >> v >> w >> k;
        g[v][w] = k;
    }
    return g;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Need at lest 2 point to search" << endl;
        return 1;
    }

    auto graph = build_graph(cin);
    
    string src_name(argv[1]);
    string dst_name(argv[2]);

    auto src_it = graph.find(src_name);
    auto dst_it = graph.find(dst_name);

    if (src_it == graph.end()) {
        cerr << "Unknown sorce: " << src_name << endl;
    }
    if (dst_it == graph.end()) {
        cerr << "Unknown destination: " << dst_name << endl;
    }

    auto result = shortest_length(graph, src_it->first, dst_it->first);
    cout << result << endl;
    return 0;
}
