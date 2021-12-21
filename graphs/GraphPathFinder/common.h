#pragma once
#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <windows.h>
#include <map>
#include <string>

//#ifdef DIJKSTRA_EXPORTS
//#BUILD_DLL
#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport) 
#endif

typedef std::map<std::string, float> weight_map;    /// �������� ������� -> ��� ����� � ���� �������
typedef std::map<std::string, weight_map> graph;    /// �������� ������� -> ����� ������, � ������� ����� ������� �� ������

/// std::string (�������� �������) -> std::string (�������� �������) -> float (��� �����)

float DLL_EXPORT shortest_length(const graph& graph, const std::string& src, const std::string& dst);


#endif // COMMON_H_INCLUDED
//���������� � ��������->������������