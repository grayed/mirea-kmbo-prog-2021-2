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
    copy(from, from + count, to);
}

template<typename T>
void print_array(const T *parr, size_t count) {
    auto f = [](int n) {cout << n << " "; };
    for_each(parr, parr + count, f);
}

template <typename T>
int min_array(const T* where, size_t count)
{
    if (!count) return 0;
    int min = 0;
    bool is_found = false;
    for (auto i = 1; i < count; i++)
        if (where[i] < where[min])
        {
            min = i;
            is_found = true;
        }
    if (!is_found) return -1;
    return min;
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
