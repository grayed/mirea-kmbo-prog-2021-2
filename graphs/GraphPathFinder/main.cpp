#include <iostream>
#include <map>
#include <string>

#include "common.h"

using namespace std;


graph build_graph(std::istream& is) {
    std::string src = "";
    std::string dst = ""; 
    float weight = 0; 

    graph g;

    while (true) {
        is >> src >> dst >> weight;
        if (src == "")
            break;
        g.insert(std::pair<std::string, weight_map>(dst, weight_map()));
        std::pair<graph::iterator, bool> insres = g.insert(std::pair<std::string, weight_map>(dst, weight_map()));
        insres.first->second.insert(std::pair<std::string, float>(dst, weight));
    }

    return g;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        cerr << "Need at least 2 points to search" << endl;
        return 1;
    }

    /// 1. Считывать описание вершин и рёбер, формируя граф в оперативной памяти
    ///     считываем построчно с stdin  (std::cin), разбираем строчку, добавляем запись
    ///     каждая строка - это три значения: исходная вершина, конечная вершина, вес
    auto graph = build_graph(cin);

    /// 2. Декодировать и верифицировать исходную и конечную вершины графа для определения пути
    ///     два аргумента командной строки
    string src_name(argv[1]);
    string dst_name(argv[2]);

    auto src_it = graph.find(src_name);
    auto dst_it = graph.find(dst_name);

    if (src_it == graph.end()) {
        cerr << "Unknown source: " << src_name << endl;
        return 1;
    }
    if (dst_it == graph.end()) {
        cerr << "Unknown destination: " << dst_name << endl;
        return 1;
    }

    /// 3. Вызвать функцию поиска пути
    ///     в библиотеке
    auto result = shortest_length(graph, src_it->first, dst_it->first);

    /// 4. Вывести результат
    cout << result << endl;
    return 0;
}
