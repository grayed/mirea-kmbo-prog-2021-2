#include <iostream>
#include <algorithm>

using std::cout;
using std::cin ;
using std::endl;

template<typename T>
void copy_arr (const T* from, T* to, size_t count) 
{
    for (auto i = 0; i < count; ++i) 
    {
        to[i] = from[i];
    }
}

template<typename T>
void print_arr(const T* pair, size_t count)
{
    for (auto i = 0; i < count; ++i)
    {
        cout << pair[i] << '\t';
    }
}

//ô-èÿ MIN(const T* where, size_t count):
//åñëè ìàññèâ ïóñò, òî ô-èÿ âîçâðàùàåò 0
//èíà÷å èíäåêñ ìèíèìàëüíîãî ýëåìåíòà
template<typename T>
size_t MIN(const T* where, size_t count)
{
    T res = where[0];
    size_t j = 0;

    for (size_t i = 1; i < count; ++i)
    {
        if (where[i] < res)
        {
            res = where[i];
            j = i;
        }
    }
    return j;
}

template<typename Iter>
Iter find_min(Iter from, Iter last)
{
    Iter it = from;
    for (Iter itr = from; itr != last; ++itr)
    {
        if (*from < *itr)
        {
            it = from;
        }
    }
    return it;
}

template<typename Iter>
Iter find_max(Iter from, Iter last)
{
    Iter it = from;
    for (Iter itr = from; itr != last; ++itr)
    {
        if (*from > *it)
        {
            it = from;
        }
    }
    return it;
}
template<typename Iter>
std::pair<Iter, Iter> min_max_element(Iter from, Iter last)
{
    std::pair<Iter, Iter> res;
    res.first  = from;
    res.second = from;

    for (Iter itr = from; itr != last; ++itr)
    {
        if (*itr > *res.first)
        {
            res.first = itr;
        }
        else if (*itr < *res.second)
        {
            res.second = itr;
        }
    }

    return res;
}

int main(int argc, char **argv)
{
    //int arr[] = { 10,9,8,7,6,1,5,4,3,2 };
    //output:       5       1
    //int arr[] = {41, 67, 34, 0, 69, 24, 78, 58, 62, 64, 5, 45, 81, 27, 61, 91, 95, 42, 27, 36}
    //output:       3       0
    //              0       95


    int arr[20];
    for (auto i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
    {
        arr[i] = rand() % 100;
    }
    
    int ind = MIN(arr, sizeof(arr) / sizeof(arr[0]));
    std::for_each(arr, arr + sizeof(arr) / sizeof(arr[0]), [](int n) {cout << n << ' '; });
    cout << '\n' << ind <<'\t' << arr[ind] << endl;

    auto res = std::min_element(arr, arr + 20);


    auto res2_pairr = std::minmax_element(arr, arr + 20);
    cout << "Min = " << *res2_pairr.first << "\t, Max = " << *res2_pairr.second << endl;
    
    auto res2_pair = min_max_element(arr, arr + 20);
    cout << "\nMax = " << *res2_pair.first << "\t, Min = " << *res2_pair.second << endl;
    return 0;
}
