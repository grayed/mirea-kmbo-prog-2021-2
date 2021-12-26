#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
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
    auto p = [](int a) {cout << a << " ";};
    for_each(parr, parr + count, p);
}

template<typename T>
size_t minimal(const T *from, size_t count) {
    if (count == 0)
        return 0;
    int ans = 0;
    for (size_t i = 1; i < count; ++i) {
        if (from[i] < from[ans]) {
            ans = i;
        }
    }
    if (ans == 0) return -1;
    else return ans;
}

int main(int argc, char **argv) {
    int arr[20], arr2[20];
    for (auto i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
        arr[i] = rand();
    copy_array(arr, arr2, sizeof(arr)/sizeof(arr[0]));
    print_array(arr, sizeof(arr)/sizeof(arr[0]));
    cout << endl;
    print_array(arr2, sizeof(arr2)/sizeof(arr2[0]));
    return 0;
}
