#include <cstdint>
#include <iostream>
#include <cstdlib>

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

template <typename R>
R mini(R* arr, int count)
{
	R a = arr[0];

	if (a<0)
	{
		a = 0;
		return a;
	}
	for(int i=0;i<count;i++)
	{
		if (arr[i] < a)
			a = arr[i];
	}
	return a;
}

int main(int argc, char **argv) {
    int* ad;
	int* mas = new int[10];
	for (int i = 0; i < 10; i++)
	{
		mas[i] = i;
	}
	int b = mini(mas, 10);
	cout << b << "\n";
	for (int i = 0; i < 10; i++)
		mas[i] = i + 16;
	b = mini(mas, 10);
	cout << b << "\n";
	if (mas[0] > 0)
	{
		ad = min_element(mas + 0, mas + 8);
		cout << (*ad) + 8 << "\n";
	}
	else
	{
		cout << 0 << "\n";
	}
    int arr[20], arr2[20];
    for (auto i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
        arr[i] = rand();
    copy_array(arr, arr2, sizeof(arr)/sizeof(arr[0]));
    print_array(arr, sizeof(arr)/sizeof(arr[0]));
    print_array(arr2, sizeof(arr2)/sizeof(arr2[0]));
    return 0;
}
