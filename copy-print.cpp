#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <list>
#include <vector>
#include <numeric>
#include <string>
// Домашнее заданиеда:
//
// 1. Дореализовать copy_array() и print_array()
//
// 2. Реализовать функцию поиска минимума в заданном массиве:
//       const T *where, size_t count, const T &what
//     Должна возвращать индекс найденного элемента (-1 если не найден,
//     но 0 для пустого массива).

using namespace std;

template<typename T>
void copy_array(const T *from, T *to, size_t count) {
    for (size_t i = 0; i < count; i++)
    {
        to[i] = from[i];
    }
}

template<typename T>
void print_array(const T *parr, size_t count) {
    for (size_t i = 0; i < count; i++)
    {
        cout << parr[i] <<"\n";
    }
}

int find_min(int* array, int len)
{
	if (len == 0)
	return 0;
	int mini = array[0];
	for (auto i = 0; i < len; i++)
	{
		if (array[i] < mini)
			mini = array[i];
	}
	return mini;
}

template<typename Iter>
Iter find_min2(Iter from, Iter till)
{
	auto itres = from;
	for (auto it = from; it != till; it++)
		if (*it < *itres)
			itres = it;
	return itres;
}

template<typename Iter>
std::pair<Iter, Iter> my_minmax_element(Iter from, Iter till)
{
	std::pair<Iter, Iter> result(from, from);
	for (auto it = from; it != till; it++)
	{
		if (*it < *result.first)
			result.first = it;
		if (*it > *result.second)
			result.second = it;
	}
	return result;
}

template <typename T>
vector<T> series(vector<vector<T>> a, int size)
{
	vector<T> b(size);
	//std::fill(b.begin(), b.end(), 0);
	for (auto i = 0; i < size; i++)
	{
		b[i] = accumulate(a[i].begin(), a[i].end(), 0);
	}
	return b;
}
template <typename V>
class MatrixReduceOperation
{
public:
	virtual vector<V> calc(vector<vector<V>> x, int size) = 0;
};

template <class V>
class MatrixRowSum : public MatrixReduceOperation<V>
{
public:
	vector<V> calc(vector<vector<V>> x, int size)
	{
		vector<V> y(size);
		//std::fill(y.begin(), y.end(), 0);
		for (auto i = 0; i < size; i++)
		{

			y[i] = accumulate(x[i].begin(), x[i].end(), 0);
		}
		return y;
	}
};

int main(int argc, char **argv) {
    srand(time(NULL));
    int arr[20], arr2[20];
    int *arr3 = new int[6];
    for (auto i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
        arr[i] = rand();
    std::copy(arr, arr + sizeof(arr) / sizeof(arr[0]), arr2);
    auto f = [](int n) {cout << n << ' '; };
    std::for_each(arr, arr + sizeof(arr) / sizeof(arr[0]), f);
    cout << "\n";
    std::cout<<"\n\n"<<find_min(arr2,20)<<"\n";
    auto min_el = min_element(arr, arr + sizeof(arr) / sizeof(arr[0]));
    std::cout << *min_el<<"\n";
    auto itpair = my_minmax_element(arr, arr + sizeof(arr) / sizeof(arr[0]));
    std::cout << *itpair.first << "\n";
    std::cout << *itpair.second << "\n";
    std::vector<int> nums, nums2;
    std::vector<vector<int>> data = { {1,2,3},{4,5,6},{7,8,9}};
    std::vector<vector<char>> data2 = {{'d','t','t'}, {'d','f','t'}, {'u','f','t'}};
    std::vector<int> a = series(data, 3);
    std::cout << a[0] << "\n" << a[1] << "\n" << a[2] << "\n";
    std::vector<char> a1 = series(data2, 3);
    std::cout << a1[0] << "\n" << a1[1] << "\n" << a1[2];
    return 0;
}
