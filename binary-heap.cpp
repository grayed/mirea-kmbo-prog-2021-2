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
    HeapOverArray(const std::vector<T> &initv) : v(initv) {}    /// Требуется, чтобы массив был заранее упорядочен

    const std::vector<T>& getVector() const { return v; }

    /// Если такой элемент уже существует, то изменения не вносятся, а функция возвращает false.
    /// Если элемент добавлен, то функция возвращает true.
    bool addNode(const T& o) {
        size_t pos = v.size();
        if (pos == 0) 
	{
            v.push_back(o);
            return true;
        }
        size_t parent_pos = (pos-1)/2;
	v.push_back(o);
        while (pos != 0 && o > v[parent_pos]) 
	{
            std::swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos = (pos - 1) / 2;
        }
	return true;
    }
	
    //добавим поиск индекса, чтобы было легче работать с удалением
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
        if (pos == -1) 
	{
            return false;
        }
        v[pos] = v.back();   
        v.pop_back();
        size_t parent_pos = (pos - 1) / 2;
        while (pos > 0 && v[pos] > v[parent_pos]) 
	{
            std::swap(v[pos], v[parent_pos]);
            pos = parent_pos; 
            parent_pos = (pos - 1) / 2;
        }
        size_t left_child_pos = (pos * 2 + 1);
        size_t right_child_pos = (pos * 2 + 2);
        while (right_child_pos < v.size() && 
                ((v[pos] < v[left_child_pos]) || v[pos] < v[right_child_pos]))
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
	{
            std::swap(v[pos], v[left_child_pos]);
        }
        return true;
    }
	
    T& top() 
    {
        assert(v.size() > 0);
        return v[0];
    }

    //добавление
    void pop() 
    {
        assert(v.size() > 0);
        T removed_element = v[0];
        removeNode(removed_element);
    }

    size_t size() const 
    {
        return v.size();
    }
   
    //освобождение
    bool empty() const 
    {
        return (v.size() == 0);
    }

    class iterator {
        std::vector<T> *v;
        size_t idx;
	 
     size_t findLeftmost(size_t pos) 
        {
            while (pos * 2 <= v->size())
            {
                pos *= 2;
            }
            return pos;
        }

        size_t findFirstEvenAncestor(size_t pos) 
        {
            while (pos % 2 == 1 && pos != 1) 
            {
                pos /= 2;
            }
            return pos;
        }

        bool hasRight(size_t pos) 
        {
            return (pos * 2 + 1 <= v->size());
        }

    public:
        iterator() : v(nullptr) {}
        iterator(std::vector<T> *v_, size_t idx_) : v(v_), idx(idx_) {}

        iterator& operator++() 
	{
           size_t pos = idx + 1;
            if (hasRight(pos)) 
            {
                pos = pos * 2 + 1;
                pos = findLeftmost(pos);
            }
            else {
                if (pos % 2 == 1) 
                {
                    pos = findFirstEvenAncestor(pos);
                }
                if (pos == 1) 
                {
                    pos = v->size() + 1;
                    v = nullptr;
                }
                else 
                {
                    pos /= 2;
                }
            }
            idx = pos - 1;
            return *this;
        }

        iterator operator++(int) {
            auto prev = *this;
            ++(*this);
            return prev;
        }

        operator bool() const { return v != nullptr; }

        const T& operator*() const { return v[idx]; }
        T& operator*() { return v[idx]; }
    };

    iterator begin() {
        if (v.empty())
            return iterator();
       size_t idx = 0;
        while (idx * 2 + 1 < v.size()) 
	{
            idx = idx * 2 + 1;
        }
        return iterator(&v, idx); 
    }

    iterator end() { return iterator(); }
};

template<class T>
bool testHeapAdd(const std::vector<T> &initial, const T &value, const std::vector<T> &expected) {
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

    /// TODO тесты для остальных операций с кучей

    return 0;
