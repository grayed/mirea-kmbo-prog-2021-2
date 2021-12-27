template<typename T>
void copy_array(const T *from, T *to, size_t count) {
    // TODO
    for (size_t i = 0; i < count; i++)
		to[i] = from[i];
}

template<typename T>
void print_array(const T *parr, size_t count) {
    // TODO
    for (size_t i = 0; i < count; i++)
		cout << parr[i] << endl;
}

template<typename T>
size_t findMin(const T* parr, size_t count)
{
	size_t min = 0;
	if (count == 0)
		return min;
	T elem = parr[0];
	for (size_t i = 1; i < count; i++)
	{
		if (parr[i] < elem)
		{
			elem = parr[i];
			min = i;
		}
	}
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
