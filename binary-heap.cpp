#include <iostream>
#include <vector>

using namespace std;

///
/// Домашнее задание:
///
/// 1. Реализовать добавление и удаление узлов в бинарную кучу.
/// 2. Реализовать итератор, проходящий от самой левой до самой правой вершины дерева.
/// 3. Написать юнит-тесты для всех перечисленных выше задач.
///


template<class T>
class HeapOverArray {
    std::vector<T> v;

public:
    HeapOverArray() {}
    HeapOverArray(const std::vector<T> &initv) : v(initv) {}

    const std::vector<T>& getVector() const { return v; }

    bool addNode(const T& o)
    {
        size_t pos = v.size();
        if (pos == 0) 
	{
            v.push_back(o);
            return true;
        }
        size_t parentpos = (pos - 1) / 2;
        v.pushback(o);
        while (pos != 0 && o > v[parentpos]) 
        {
            std::swap(v[pos], v[parentpos]);
            pos = parentpos;
            parentpos = (pos - 1) / 2;
        }
	return true;
    }

    bool removeNode(const T& o)
    {
	size_t pos = findIndex(o);
        if (pos == -1)
            return false;
        v[pos] = v.back();
        v.pop_back();
	size_t parentpos = (pos - 1) / 2;
        while (pos > 0 && v[pos] > v[parentpos]) 
        {
            std::swap(v[pos], v[parentpos]);
            pos = parentpos;
            parentpos = (pos - 1) / 2;
        }
        size_t left_childpos = (pos * 2 + 1);
        size_t right_childpos = (pos * 2 + 2);
        while (right_childpos < v.size() && ((v[pos] < v[left_childpos]) || v[pos] < v[right_childpos]))
        {
            if (v[left_childpos] >= v[right_childpos]) 
            {
                std::swap(v[pos], v[left_childpos]);
                pos = left_childpos;
            }
            else 
            {
                std::swap(v[pos], v[right_childpos]);
                pos = right_childpos;
            }
            left_childpos = (pos * 2 + 1);
            right_childpos = (pos * 2 + 2);
        }
        if (left_childpos < v.size() && v[pos] < v[left_childpos])
            std::swap(v[pos], v[left_childpos]);

        return true;
    }
	
    T& top() 
    {
        assert(v.size() > 0);
        return v[0];
    }

    const T& top() const 
    {
        assert(v.size() > 0);
        return v[0];
    }

    void pop() 
    {
        assert(v.size() > 0);
        T removed_elem = v[0];
        removeNode(removed_elem);
    }

    size_t size() const { return v.size(); }

    bool empty() const { return (v.size() == 0); }

    class iterator 
    {
        std::vector<T> *v;
        size_t idx;
	    
        size_t findLeft(size_t pos) 
        {
            while (pos * 2 <= v->size())
                pos *= 2;
            return pos;
        }

        size_t findAncestor(size_t pos) 
        {
            while (pos % 2 == 1 && pos != 1)
                pos /= 2;
            return pos;
        }

        bool findRight(size_t pos) 
        {
            return (pos * 2 + 1 <= v->size());
        }


    public:
        iterator() : v(nullptr) {}
        iterator(std::vector<T> *v_, size_t idx_) : v(v_), idx(idx_) {}

        iterator& operator++()
	{
            size_t pos = idx + 1;
            if (findRight(pos)) 
            {
                pos = pos * 2 + 1;
                pos = findLeft(pos);
            }
            else 
            {
                if (pos % 2 == 1)
                    pos = findAncestor(pos);
                if (pos == 1) 
                {
                    pos = v->size() + 1;
                    v = nullptr;
                }
                else
                    pos /= 2;
            }
            idx = pos - 1;
            return *this;
        }

        iterator operator++(int)
	{
            auto prev = *this;
            ++(*this);
            return prev;
        }

        operator bool() const { return v != nullptr; }

        const T& operator*() const { return v[idx]; }
        T& operator*() { return v[idx]; }
    };

    iterator begin()
    {
        if (v.empty())
            return iterator();
        size_t idx = 0;
        while (idx * 2 + 1 < v.size())
            idx = idx * 2 + 1;
        return iterator(&v, idx);
    }

    iterator end() { return iterator(); }
};

template<class T>
bool testHeapAdd(const std::vector<T> &initial, const T &value, const std::vector<T> &expected)
{
    HeapOverArray<T> heap(initial);
    heap.addNode(value);
    auto v = heap.getVector();
    if (v.size() != expected.size())
    {
        std::cerr << "size difference: expected " << expected.size() << ", got " << v.size() << std::endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++)
        if (v[i] != expected[i])
	{
            std::cerr << "difference in " << i << ": expected " << expected[i] << ", got " << v[i] << std::endl;
            return false;
        }
    return true;
}

template<class T>
bool testHeapIterator(const std::vector<T>& initial, const std::vector<T>& expected)
{
    HeapOverArray<T> heap(initial);
    std::vector<T> v;
    for (auto it = heap.begin(); it; it++)
        v.push_back(*it);
    if (v.size() != expected.size()) 
    {
        std::cerr << "size difference: expected " << expected.size() << ", got " << v.size() << std::endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++) 
    {
        if (v[i] != expected[i]) 
        {
            std::cerr << "diffecrence in " << i << ": expected " << expected[i] << ", got " << v[i] << std::endl;
            return false;
        }
    }
    return true;
}

template<class T>
bool testHeapRemove(const std::vector<T>& initial, const T& value, const std::vector<T>& expected)
{
    HeapOverArray<T> heap(initial);
    heap.removeNode(value);
    auto v = heap.getVector();
    if (v.size() != expected.size())
    {
        std::cerr << "size difference: expected " << expected.size() << ", got " << v.size() << std::endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++) 
    {
        if (v[i] != expected[i]) 
        {
            std::cerr << "diffecrence in " << i << ": expected " << expected[i] << ", got " << v[i] << std::endl;
            return false;
        }
    }
    return true;
}

int main()
{
    vector<int> initial = { 7, 3, 4 };
    vector<int> expected = { 8, 4, 7, 3 };
    testHeapAdd(initial, 8, expected);

    initial = { 100, 80, 60, 20, 10, 40, 5, 8, 2, 9, 1, 3 };
    expected = { 8, 20, 2, 80, 9, 10, 1, 100, 3, 40, 60, 5 };
    testHeapIterator(initial, expected);

    initial = { 100, 80, 60, 20, 10, 40, 5, 8, 2, 9, 1, 3 };
    expected = { 100, 20, 60, 8, 10, 40, 5, 3, 2, 9, 1 };
    testHeapRemove(initial, 80, expected);

    initial = { 100, 80, 60, 20, 10, 40, 5, 8, 2, 9, 1, 30 };
    expected = { 100, 30, 60, 20, 10, 40, 5, 8, 2, 9, 1 };
    testHeapRemove(initial, 80, expected);

    return 0;
}
