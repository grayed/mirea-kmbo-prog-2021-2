#include "pch.h"

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

float shortest_length(const graph& graph, const std::string& src, const std::string& dst) {
    map<string, float> labels;
    set<string> tovisit;

    /// Проставить метки
    for (const auto& kv : graph) {
        if (kv.first == src) {
            labels[kv.first] = 0;
            tovisit.insert(kv.first);
        } 
        else
            labels[kv.first] = std::numeric_limits<float>::infinity();
        //tovisit.insert(kv.first);
    }
    // за O(ElogV)
    while (tovisit.size() > 0) {
        /// 1. Выбираем не посещённую вершину с наименьшей меткой
        auto vi = std::min_element(tovisit.begin(), tovisit.end(),
            [labels](const std::string& v1, const std::string& v2) { return labels.at(v1) < labels.at(v2); });
        tovisit.erase(*vi);
        /// 2. Вычисляем метки для соседей, для каждого соседа записываем вычисленную метку, если она меньше существующей
        for (const auto& neighbor : graph.at(*vi)) {
            if (tovisit.find(neighbor.first) == tovisit.end())
                continue;   /// вершина уже была посещена

            /// neighbor.first - имя соседней вершины
            /// neighbor.second - расстояние до соседней вершины
            if (labels[neighbor.first] > labels[*vi] + neighbor.second) {
                tovisit.erase(neighbor.first);
                labels[neighbor.first] = labels[*vi] + neighbor.second;
                tovisit.insert(neighbor.first);
            }

        }
    }

    return 0;
}
