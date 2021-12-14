#include <iostream>
#include <cstdint>
#include <ctime>
#include <algorithm>
using namespace std;

template<typename T>
void copy_array(const T* from, T* to, size_t count) {
    while (count--)
        *to++ = *from++;
}

template<typename T>
void print_array(const T* parr, size_t count) {
    while (count--)
        cout << *parr++ << " ";
    cout << endl;
}

template<typename T>
T find_min(const T* where, size_t count) {
    if (count == 0)
        return 0;
    T min = where[0];
    while (count--) {
        if (*where < min)
            min = *where;
        *where++;
    }
    return min;
}

int main() {
    srand(time(NULL));
    int arr[20], arr2[20];
    for (auto i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        arr[i] = rand() % 100;
    copy(arr, arr + sizeof(arr2) / sizeof(arr[0]), arr2);

    auto f = [](int n) {cout << n << " "; };
    for_each(arr, arr + sizeof(arr2) / sizeof(arr[0]), f);
    cout << endl;

    print_array(arr2, sizeof(arr2) / sizeof(arr[0]));
    cout << find_min(arr2, sizeof(arr2) / sizeof(arr[0]));

    char c; cin >> c;
    return 0;
}