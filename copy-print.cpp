#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <algorithm>

template<typename T>
void print_array(const T parr, size_t count)
{
	for (size_t i = 0; i < count; i++)
		std::cout << parr[i] << std::endl;
}

template<typename T>
void copy_array(const T* from, T* to, size_t count)
{
	for (size_t i = 0; i < count; i++)
		to[i] = from[i];
}

// Функция поиска минимума в массиве
template<typename T>
T func_min(T* arr, size_t count)
{
	if (count == 0)
		return 0;
	T min = arr[0];
	for (size_t i = 1; i < count; i++)
	{
		if (arr[i] < min) min = arr[i];
	}
	return min;
}

int main(int argc, char** argv)
{
	int arr[20], arr2[20];
	for (auto i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
		arr[i] = rand();
	copy_array(arr, arr2, sizeof(arr) / sizeof(arr[0]));
	print_array(arr, sizeof(arr) / sizeof(arr[0]));
	print_array(arr2, sizeof(arr2) / sizeof(arr2[0]));
	std::cout << func_min(arr, sizeof(arr) / sizeof(arr[0]));
	return 0;
}

