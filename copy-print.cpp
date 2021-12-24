// 1. Дореализовать copy_array() и print_array()
//
// 2. Реализовать функцию поиска минимума в заданном массиве:
//       const T *where, size_t count, const T &what
//     Должна возвращать индекс найденного элемента (-1 если не найден,
//     но 0 для пустого массива).

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <algorithm>

using namespace std;

template<typename T>
void copy_array(const T *from, T *to, size_t count) {
    for (int i = 0; i < count; i++)
        to[i] = from[i];
}

template<typename T>
void print_array(const T *parr, size_t count){
    for (int i = 0; i < count; i++)
        cout << parr[i] << " ";
    cout << "\n";
}

template<typename T>
T min_finder(const T *parr, size_t count){
    T minimum = parr[0];
    for (int i = 1; i < count; i++)
        if (minimum > parr[i])
            minimum = parr[i];
    return minimum;
}

int main(int argc, char** argv) {
    int arr[20], arr2[20];
    srand(1);
    for (auto i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
        arr[i] = rand();
    copy_array(arr, arr2, sizeof(arr)/sizeof(arr[0]));
    //auto f = [] (int n) { cout << n << " "; };
    //for_each(arr, arr + sizeof(arr)/sizeof(arr[0]), f);
    print_array(arr, sizeof(arr)/sizeof(arr[0]));
    print_array(arr2, sizeof(arr2)/sizeof(arr2[0]));
    cout << min_finder(arr, sizeof(arr)/sizeof(arr[0]));
    for (auto i = 0; i < argc; i++)
        cout << argv[i] << endl;
    return 0;
}
