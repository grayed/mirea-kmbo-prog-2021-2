#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <map>
#include <string>

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

typedef map<string, float> weight_map;
typedef map<string, weight_map> graph;

float DLL_EXPORT shortest_length(const graph &graph, const weight_map &src, const weight_map &dst);

#endif // __MAIN_H__
