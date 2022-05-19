#include <cstdint>
#include <iostream>
#include <cstdlib>

// Домашнее задание:
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
		to[i] = from[i];
}

template<typename T>
void print_array(const T *parr, size_t count) {
    for (size_t i = 0; i < count; i++)
		cout << parr[i] << endl;
}

template<typename T>
size_t FindMin(const T* parr, size_t count)
{
	size_t min = 0;
	if (count == 0)
		return min;
	T elem = parr[0];
	for (size_t i = 0; i < count; i++)
	{
		if (parr[i] < elem)
		{
			elem = parr[i];
			min = i;
		}
	}
	return min;
}

int main() {
	const int array_size = 20;
    int arr[array_size], arr2[array_size];
    for (auto i = 0; i < array_size; i++)
        arr[i] = rand();
    copy_array(arr, arr2, array_size);
    print_array(arr, array_size);
    print_array(arr2, array_size);
    return 0;
}
