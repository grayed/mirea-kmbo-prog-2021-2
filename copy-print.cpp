#include <cstdint>
#include <iostream>
#include <cstdlib>

using namespace std;

template<typename T>
void copy_array(const T *from, T *to, size_t count) {
    for (size_t i = 0; i < count; i++) {
        to[i] = from[i];
    }
}

template<typename T>
void print_array(const T *parr, size_t count) {
    for (size_t i = 0; i < count; i++) {
        cout << parr[i] << " ";
    }
    cout << endl;
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
