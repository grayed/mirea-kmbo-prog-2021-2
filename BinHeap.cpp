#include <iostream>
#include <vector>
using namespace std;

template <class T>
class HeapOverArray
{
    vector<T> v;
public:
    vector<T> getVector() { return v; }
    HeapOverArray() {}
    HeapOverArray(vector<T> a) { v = a; }
    bool addNode(const T& o)
    {
        size_t pos = v.size();
        if (pos == 0) {
            v.push_back(o);
            return true;
        }
        size_t parent_pos = (pos - 1) / 2;
        v.push_back(o);
        while (pos != 0 && o > v[parent_pos]) {
            std::swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos = (pos - 1) / 2;
        }
        return true;
    }
    size_t findIndex(const T& o) {
        for (size_t i = 0; i < v.size(); i++) {
            if (v[i] == o) {
                return i;
            }
        }
        return -1;
    }

    bool removeNode(const T& o) {
        size_t pos = findIndex(o);
        if (pos == -1) {
            return false;
        }
        v[pos] = v.back();
        v.pop_back();
        size_t parent_pos = (pos - 1) / 2;
        while (pos > 0 && v[pos] > v[parent_pos]) {
            std::swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos = (pos - 1) / 2;
        }
        size_t left_heir = (pos * 2 + 1);
        size_t right_heir = (pos * 2 + 2);
        while (right_heir < v.size() &&
            ((v[pos] < v[left_heir]) || v[pos] < v[right_heir]))
        {
            if (v[left_heir] >= v[right_heir]) {
                std::swap(v[pos], v[left_heir]);
                pos = left_heir;
            }
            else {
                std::swap(v[pos], v[right_heir]);
                pos = right_heir;
            }
            left_heir = (pos * 2 + 1);
            right_heir = (pos * 2 + 2);
        }
        if (left_heir < v.size() && v[pos] < v[left_heir]) {
            std::swap(v[pos], v[left_heir]);
        }
        return true;
    }
};

template<class T>
bool testHeapAdd(const vector<T>& initial, const T& value, const vector<T>& expected)
{
    HeapOverArray<T> a(initial);
    a.addNode(value);
    auto v = a.getVector();
    if (v.size() != expected.size())
    {
        cerr << "size difference: expected " << expected.size() << ", got" << v.size() << '\n';
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++)
    {
        if (v[i] != expected[i])
        {
            cerr << " difference in " << i << " expected: " << expected[i] << ", got" << v[i] << '\n';
            return false;
        }
    }
    return true;
}

template<class T>
bool testHeapRem(const vector<T>& initial, const T& value, const vector<T>& expected)
{
    HeapOverArray<T> a(initial);
    a.removeNode(value);
    auto v = a.getVector();
    if (v.size() != expected.size())
    {
        cerr << "size difference: expected " << expected.size() << ", got" << v.size() << '\n';
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++)
    {
        if (v[i] != expected[i])
        {
            cerr << " difference in " << i << " expected: " << expected[i] << ", got" << v[i] << '\n';
            return false;
        }
    }
    return true;
}


template<class T>
class iterator
{
    vector<T>* v;
    size_t idx;
public:
    iterator() : v(nullptr) {}
    iterator(std::vector<T>* v_, size_t idx_) : v(v_), idx(idx_) {}
    size_t getindex() { return idx; }
    iterator& operator++()
    {
        if (((idx + 1) * 2 + 1) > (*v).size())
        {
            while ((idx + 1) % 2 != 0)
            {
                idx = idx / 2 - 1;
            }
            if (idx == 0)
                v = nullptr;
            idx = idx / 2;
        }
        else
        {
            idx = idx * 2 + 2;
            while (idx * 2 + 1 < (*v).size())
                idx = idx * 2 + 1;
        }
        return *this;
    }

    iterator operator++(int) {
        auto prev = *this;
        ++(*this);
        return prev;
    }

    const T& operator*() const { return v[idx]; }
    T& operator*() { return v[idx]; }
};

template<class T>
bool testHeapIterator(const vector<T>& initial, int const& indx, const vector<T>& expected)
{
    HeapOverArray<T> a(initial);
    auto v = a.getVector();
    ::iterator<T> b(&v, indx);
    for (auto i = 0; i < expected.size(); i++)
    {
        if (v[b.getindex()] != expected[i])
        {
            cerr << " difference in " << i << " expected: " << expected[i] << ", got " << v[i] << '\n';
            return false;
        }
        b++;
    }
    return true;
}



int main()
{
    vector<int> initial = { 7, 3, 4 };
    vector<int> expected = { 8, 4, 7, 3 };
    testHeapAdd(initial, 8, expected);

    vector <int> initial_2{ 50, 12, 18, 8, 9, 6, 8, 2, 4 };
    vector <int> expected_2{ 31, 17, 30, 5, 12, 7, 9, 1, 3, 9 };
    testHeapAdd(initial_2, 21, expected_2);
    initial_2 = { 7, 3, 4 };
    expected_2 = { 8,4,6,3 };
    testHeapAdd(initial_2, 5, expected_2);
    testHeapRem(initial_2, 3, expected_2);
    initial_2 = expected_2;
    expected_2 = { 20, 12, 15, 1, 9, 7, 10 };
    testHeapRem(initial_2, 6, expected_2);
    initial_2 = expected_2;
    expected_2 = { 21, 16, 8, 1, 8, 6};
    testHeapRem(initial_2, 15, expected_2);
    initial_2 = expected_2;
    expected_2 = { 11, 9, 8, 1,7 };
    testHeapRem(initial_2, 20, expected_2);
    initial_2 = expected_2;
    testHeapRem(initial_2, 20, expected_2);
    return 0;
}