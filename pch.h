// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H


#include <windows.h>
#include <map>
#include <string>



#ifdef FORDBELL_EXPORTS
#define FORDBELL_EXPORTS __declspec(dllexport)
#else
#define FORDBELL_EXPORTS __declspec(dllimport)
#endif

typedef std::map<std::string, float> weight_map;    /// название вершины -> вес ребра к этой вершине
typedef std::map<std::string, weight_map> graph;    /// название вершины -> набор вершин, в которые можно попасть из данной

float FORDBELL_EXPORTS shortest_length(const graph& graph, const std::string& src, const std::string& dst);



#include "framework.h"

#endif PCH_H
