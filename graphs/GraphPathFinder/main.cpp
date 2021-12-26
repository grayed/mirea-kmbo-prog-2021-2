#include <iostream>
#include <map>
#include <string>

#include "common.h"

using namespace std;

/// вершина -> { вершина -> вес }


graph build_graph(std::istream& is) {
    std::string src = ""; /// исходная вершина
    std::string dst = ""; /// конечная вершина
    float weight = 0;    /// вес ребра src-dst



    graph graph;

    /// цикл до конца входного потока данных
    while (!is.eof()) {
        /// FIXME лучше через is.get_line() (например, обернув считанную строку в std::stringstream)
        is >> src >> dst >> weight;

        /// проверяем, существует ли конечная вершина; если нет - добавляем; если да, используем имеющуюся
        if (graph.count(dst) == 0) {
            graph.insert(std::pair<std::string, weight_map>(dst, weight_map()));
        }

        /// проверяем, существует ли исходная вершина; если нет - добавляем; если да, используем имеющуюся
        if (graph.count(src) == 0) {
            graph.insert(std::pair<std::string, weight_map>(src, weight_map()));
        }

        std::pair<graph::iterator, bool> insres = graph.insert(std::pair<std::string, weight_map>(dst, weight_map()));
       
        /// добавляем запись про конечную вершину и вес в исходную
        insres.first->second.insert(std::pair<std::string, float>(dst, weight));

        /// переходим к следующей строке
    }
    return graph;
}

int main(int argc, char** argv)
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
