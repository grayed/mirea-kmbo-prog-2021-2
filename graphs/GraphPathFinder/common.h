#pragma once
#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <windows.h>
#include <map>
#include <string>

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

typedef std::map<std::string, float> weight_map;    /// название вершины -> вес ребра к этой вершине
typedef std::map<std::string, weight_map> graph;    /// название вершины -> набор вершин, в которые можно попасть из данной

/// std::string (исходная вершина) -> std::string (конечная вершина) -> float (вес ребра)

float DLL_EXPORT shortest_length(const graph &graph, const std::string &src, const std::string &dst);


#endif // COMMON_H_INCLUDED
