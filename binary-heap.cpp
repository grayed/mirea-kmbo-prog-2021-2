#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


template<class T>
class HeapOverArray {
    std::vector<T> v;

    //������� ���������
    void HeapMaxify(size_t ind, size_t size = v.size())
    {

        //https://habr.com/ru/post/587228/

        //���������� ����� � ������ �������� ����
        size_t  left   = 2 * ind
                ,right = left + 1;

        //������������, ��� �� ������, ��������
        size_t largest = ind;

        //���� ����� �������� ���� ������ ��� ������
        //���������(������ � �������� largest)
        //�� �� �������� largest �� ������ ������
        //��� ������� ��������� ����
        if (left < size && v[left] > v[largest]) {
            largest = left;
        }
        if (right < size && v[right] > v[largest]) {
            largest = right;
        }

        //���� ��-���� �������� ���������� ������������ 
        //���� � �������� i �� �������� ����� �������
        //�� ����������� ���� � ���� � ������� ��������� �������� �������
        if (largest != ind) {
            std::swap(v, ind, largest);

            //���������� �������� �������, ����� � ������ ����
            //��������� ������� ���� �� ��������
            HeapMaxify(largest, size);
        }
    }

public:
    HeapOverArray() {}
    HeapOverArray(const std::vector<T>& initv) : v(initv) {}    /// ���������, ����� ������ ��� ������� ����������

    const std::vector<T>& getVector() const { return v; }

    /// ���� ����� ������� ��� ����������, �� ��������� �� ��������, � ������� ���������� false.
    /// ���� ������� ��������, �� ������� ���������� true.
       

        bool addNode(const T & o) {
            /// 1. ���������� ����� ������� (������ ��������� ����� � ������ �������������� ����).
       /// 2. ���������� � ���������, ���� �������� ������ �������������, ������ ������� � ��������� (� �����).
            size_t pos = v.size();

            if (pos == 0)
            {
                v.push_back(o);
                return true;
            }
            else if (v[0] >= o)
                for (int i = 0; i < pos; i++)
                    if (v[i] == o) return false;

            size_t parent_pos = (pos + 1) / 2 - 1;

            while (o > v[parent_pos] && pos != 0) {
                std::swap(v.at(pos), v.at(parent_pos));
                pos = parent_pos;
                parent_pos = (pos + 1) / 2 - 1;
            }
            return true;
        }

    /// ������, �.2
    ///  1. �������� �������� �� �������� ����
    ///  2. ������������ ������������ ����������
    ///  3. ���������� ��������� ��� �������� ����
    ///

    bool removeNode(const T& o) 
    {
        auto it = std::find(v.begin(), v.end(), o); //���������� �������� �� ������ �������

        if (it != o) { //������ �������� ���
            return false;
        }
        else if (it != v.end()) //������� �� ��������� �������
        {
            //������ ������ ��� ������� � ��������� ������� � �������
            std::swap(it, v.rbegin());
            v.pop_back();
            v.shrink_to_fit(); //�������� ������

            //��������������� ������� ??

            
        }
        else //��������� �������
        {
            v.pop_back();
            v.shrink_to_fit(); //������� ���������������� ������������
            return true;
        }
            
    }

    class iterator {
        std::vector<T>* v;
        size_t idx;

    public:
        iterator() : v(nullptr) {}
        iterator(std::vector<T>* v_, size_t idx_) : v(v_), idx(idx_) {}

        ///
        /// 100
        ///  70          60
        ///  25    10    40     5
        ///   8  2  9  1  3
        ///
        /// 100 70 60 25 10 40 5 8 2 9 1 3
        ///

        iterator& operator++() {
            size_t size = ++idx;
            if (2 * size + 1 > v->size()) {
                while (size % 2 != 0) {
                    size = size / 2;
                }
                if (!(size - 1))   
                    v = nullptr;
                size = size / 2;
            }
            else
            {
                size = 2 * size + 1;
                while (2 * size + 2 <= v->size()) {
                    while (size % 2 != 0) {
                        size = *2;
                    }
                }
            }
            /// 1. ����� ������ ��������
            /// 2. �����, ����� �� ��� ������

            /// ���� ��������� ����� (���� ��� ��������� �������� ������),
            /// ����� ����� � ������ �������,
            /// ����� ���������� ������� �����, � ����� ���� �� ����� �� ������� ��������,
            /// ����� ��� ��� ����� ����� �������.
            ///
            /// ���� ��������� ����, ��: 1) ����������� ����� � 2 ���� � ���������� 1;
            /// 2) ���� ���� �������� ��������, ����������� ����� � 2 ����.
            ///
            /// ���� ��������� ������, ���������� �������� � ���������� ���� ��������� v.
            ///
            /// ���� == ����� ��������, ���������� � 2 ����, ������ ���������� ��������� � ������

            return *this;
        }

        iterator operator++(int) {
            auto prev = *this;
            ++(*this);
            return prev;
        }

        operator bool() const { return v != nullptr; }
        /// ������ ����� ������ ���:
        ///
        /// HeapOverArray::iterator iter;
        /// if (iter) { /* ... */ }
        ///
        /// HeapOverArray heap;
        /// for (auto it = heap.begin(); it; it++) { /* ... */ }

        const T& operator*() const { return v[idx]; }
        T& operator*() { return v[idx]; }
    };

    iterator begin() {
        if (v.empty())
            return iterator();
        /// �� ����� ��������� ����� �� �����
        /// TODO
    }

    iterator end() { return iterator(); }

    ///   #1 #2 #3 #4
    ///    7  3  4
    ///    7  3  4  8
    ///    7  8  4  3
    ///    8  7  4  3
    ///    8  4  7  3
};

template<class T>
bool testHeapAdd(const std::vector<T>& initial, const T& value, const std::vector<T>& expected) {
    HeapOverArray<T> heap(initial);
    heap.addNode(value);
    auto v = heap.getVector();
    if (v.size() != expected.size()) {
        std::cerr << "size difference: expected " << expected.size() << ", got " << v.size() << std::endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++)
        if (v[i] != expected[i]) {
            std::cerr << "difference in " << i << ": expected " << expected[i] << ", got " << v[i] << std::endl;
            return false;
        }
    return true;
}

int main()
{
    vector<int> initial = { 7, 3, 4 };
    vector<int> expected = { 8, 4, 7, 3 };
    testHeapAdd(initial, 8, expected);
    return 0;
}