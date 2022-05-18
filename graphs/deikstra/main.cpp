#include <iostream>
#include <vector>
#include <stack>
#include <istream>

using namespace std;

typedef vector<vector<int>> graph;

graph init_graph(istream& input)
{
    size_t nvertice;
    cout << "Количество вершин: ";
    cin >> nvertice;

    size_t nedges;
    cout << "\nКоличество ребер: ";
    cin >> nedges;

    cout << "\n";

    graph graph_matrix;
    vector<int> tmp;

    for (size_t i = 0; i < nvertice; i++)
    {
        for (size_t j = 0; j < nvertice; j++)
        {
            if (j == i)
                tmp.push_back(0);
            else
                tmp.push_back(-1);
        }
        graph_matrix.push_back(tmp);
        tmp.clear();
    }

    size_t start;
    size_t end;
    int weight;

    for (size_t i = 0; i < nedges; i++)
    {
        input >> start >> end >> weight;
        graph_matrix[start][end] = weight;
    }

    return graph_matrix;
}


vector<size_t> Dijkstra(graph g, size_t start, size_t end)
{
    vector<size_t> path;
    vector<int> min_path;
    vector<int> tmp;
    size_t nvertice = g.size();

    for (size_t j = 0; j < nvertice; j++)
    {
        min_path.push_back(-1);
    }

    min_path[start] = 0;
    bool* discovered = new bool[nvertice];
    bool* proceed = new bool[nvertice];

    for (size_t i = 0; i < nvertice; i++)
    {
        discovered[i] = false;
        proceed[i] = false;
    }

    stack<size_t> need_to_visit;
    need_to_visit.push(start);
    discovered[start] = true;

    while (!need_to_visit.empty())
    {
        size_t vertice = need_to_visit.top();
        need_to_visit.pop();
        proceed[vertice] = true;

        for (size_t i = 0; i < nvertice; i++)
        {
            if (discovered[i] == false && g[vertice][i] > 0)
            {
                need_to_visit.push(i);
                discovered[i] = true;
            }
        }
        for (size_t i = 0; i < nvertice; i++)
        {
            if ((min_path[vertice] + g[vertice][i] < min_path[i] || min_path[i] < 1) && g[vertice][i] != -1)
                min_path[i] = min_path[vertice] + g[vertice][i];
        }
    }

    for (size_t i = 0; i < nvertice; i++)
        proceed[i] = false;

    proceed[end] = true;
    path.push_back(end);

    while (end != start)
    {
        for (size_t i = 0; i < nvertice; i++)
        {
            if (g[i][end] > 0 && !proceed[i])
            {
                int temp = min_path[end] - g[i][end];
                if (temp == min_path[i])
                {
                    end = i;
                    proceed[i] = true;
                    path.push_back(i);
                    break;
                }
            }
        }
    }

    reverse(path.begin(), path.end());
    return path;
}


void print_graph(graph g)
{
    for (auto it : g)
    {
        for (auto w : it)
            cout << w << " ";
        cout << "\n";
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    graph g = init_graph(cin);
    print_graph(g);

    auto path = Dijkstra(g, 0, 3);
    for (auto it : path)
    {
        cout << it << " ";
    }
}
