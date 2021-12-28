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
        cout << parr[i] <<" ";
    cout <<"\n";
}

template <typename T>
T minimum(T* arr, int count) {
	T a = arr[0];
	if (a<0) {
		a = 0;
		return a;
	}
	for(int i=0;i<count;i++)
		if (arr[i] < a)
			a = arr[i];
	return a;
}

int main(int argc, char **argv) {
    int arr[20], arr2[20];
    for (auto i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
        arr[i] = rand();
    copy_array(arr, arr2, sizeof(arr)/sizeof(arr[0]));
    print_array(arr, sizeof(arr)/sizeof(arr[0]));
    print_array(arr2, sizeof(arr2)/sizeof(arr2[0]));
    return 0;
}
