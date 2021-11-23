#include <iostream>
#include <map>
#include <string>

#include "common.h"

using namespace std;

/// вершина -> { вершина -> вес }


graph build_graph(const std::istream &is) {
    /// TODO 1
    /// считываем строчку ребра; берём из неё названия начальной и конечной вершин ребра, а также вес ребра
    /// проверяем, существует ли исходная вершина; если нет - добавляем; если да, используем имеющуюся
    /// проверяем, существует ли конечная вершина; если нет - добавляем; если да, используем имеющуюся
    /// добавляем запись про конечную вершину и вес в исходную
    /// переходим к следующей строке
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
    auto result = shortest_length(graph, src_it->second, dst_it->second);

    /// 4. Вывести результат
    cout << result << endl;
    return 0;
}
