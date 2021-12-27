#include <iostream>
#include <map>
#include <string>

#include "common.h"

using namespace std;

/// âåðøèíà -> { âåðøèíà -> âåñ }


graph build_graph(std::istream& is) {
    graph g;
    int ver;    //maybe long int?
    is >> ver;
    for (auto i = 0; i < ver; i++) {
        string st1, st2;
        float k;
        is >> st1 >> st2 >> k;
        g[st1][st2] = k;    //2-dim array
    }
    return g;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Need at lest 2 point to search" << endl;
        return 1;
    }

    auto graph = build_graph(cin);
    //smth
    string src_name(argv[1]);
    string dst_name(argv[2]);

    auto src_it = graph.find(src_name);
    auto dst_it = graph.find(dst_name);

    if (src_it == graph.end()) {
        cerr << "Unknown sorce: " << src_name << endl;  //check exception?
    }
    if (dst_it == graph.end()) {
        cerr << "Unknown destination: " << dst_name << endl;
    }

    auto result = shortest_length(graph, src_it->first, dst_it->first);

    cout << result << endl;
    return 0;
}
