#include <iostream>
#include <vector>

using namespace std; 

template<class T>
class HeapOverArray 
{
    std::vector<T> v;

public:
    HeapOverArray() {}
    HeapOverArray(const std::vector<T>& initv) : v(initv) {}

    const std::vector<T>& getVector() const { return v; }
    bool addNode(const T& o) 
    {
        size_t pos = v.size();
        v.push_back(o);
        if (pos == 0) 
            return true;
        size_t parent_pos = (pos - 1) / 2;
        while (pos > 0 && o > v[parent_pos])
        {
            std::swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos = (pos - 1) / 2;
        }
        return true;
    }


    bool removeNode(const T& o)
    {
        size_t pos = -1;
        for (int i = 0; i < v.size(); i++)
        {
            if (v[i] == o)
                pos = i;
        }
        if (pos == -1)
            return false;
        v.pop_back();
        if (pos == v.size() - 1)
            return true;

        v[pos] = v[v.size() - 1];

        size_t parent_pos = (pos - 1) / 2;
        while (v[pos] > v[parent_pos] && pos > 0)
        {
            std::swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos = (pos - 1) / 2;
        }

        size_t left_child_pos = (pos * 2 + 1);
        size_t right_child_pos = (pos * 2 + 2);
        while (right_child_pos < v.size() && (v[left_child_pos] > v[pos] || v[right_child_pos] > v[pos]))
        {
            if (v[left_child_pos] >= v[right_child_pos]) 
            {
                std::swap(v[pos], v[left_child_pos]);
                pos = left_child_pos;
            }
            else 
            {
                std::swap(v[pos], v[right_child_pos]);
                pos = right_child_pos;
            }
            left_child_pos = (pos * 2 + 1);
            right_child_pos = (pos * 2 + 2);
        }

        if (left_child_pos < v.size() && v[pos] < v[left_child_pos])
            std::swap(v[pos], v[left_child_pos]);

        return true;
    }

    class iterator 
    {
        std::vector<T>* v;
        size_t idx;

    public:
        iterator() : v(nullptr) {}
        iterator(std::vector<T>* v_, size_t idx_) : v(v_), idx(idx_) {}

        iterator& operator++() 
        {
            size_t pos = idx + 1;
            if (pos * 2 + 1 > v->size())
            {
                while (pos % 2 != 0)
                    pos /= 2;

                if (pos == 0)
                    v = nullptr;
                pos /= 2;
            }
            else
            {
                pos = pos * 2 + 1;
                while (pos * 2 + 2 <= v->size())
                {
                    while (pos % 2 != 0)
                        pos *= 2;
                }
            }
            return *this;
        }

        iterator operator++(int) 
        {
            auto prev = *this;
            ++(*this);
            return prev;
        }

        operator bool() const { return v != nullptr; }

        const T& operator*() const { return (*v)[idx]; }
        T& operator*() { return (*v)[idx]; }
    };

    iterator begin() 
    {
        if (v.empty())
            return iterator();
        size_t idx = 0;
        while (idx * 2 < v.size())
            idx = idx * 2 + 1;
        return iterator(&v, idx);
    }

    iterator end() { return iterator(); }
};

template<class T>
bool testHeapAdd(const vector<T>& initial, const T& value, const vector<T>& expected)
{
    HeapOverArray<T> a(initial);
    a.addNode(value);
    auto v = a.getVector();
    if (v.size() != expected.size())
    {
        cerr << "size difference: expected " << expected.size() << ", got " << v.size() << '\n';
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++)
    {
        if (v[i] != expected[i])
        {
            cerr << " difference in " << i << " expected: " << expected[i] << ", got " << v[i] << '\n';
            return false;
        }
    }
    return true;
}

template<class T>
bool testHeapRemove(const vector<T>& initial, const T& value, const vector<T>& expected)
{
    HeapOverArray<T> a(initial);
    a.removeNode(value);
    auto v = a.getVector();
    if (v.size() != expected.size())
    {
        cerr << "size difference: expected " << expected.size() << ", got " << v.size() << '\n';
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++)
    {
        if (v[i] != expected[i])
        {
            cerr << " difference in " << i << " expected: " << expected[i] << ", got " << v[i] << '\n';
            return false;
        }
    }
    return true;
}

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

    initial = { 26, 15, 5, 24, 17, 8, 9, 11};
    expected = { 31, 17, 24, 27, 12, 10, 2};
    testHeapAdd(initial, 21, expected);
    initial = { 10, 7, 2, 4, 6 };
    expected = { 12, 9, 4, 6, 5, 2, };
    testHeapAdd(initial, 5, expected);
    testHeapRemove(initial, 2, expected);

    return 0; 
}
