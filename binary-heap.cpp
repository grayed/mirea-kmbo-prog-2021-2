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
        v.push_back(o);
        if (pos == 0) //дерево пустое
        {
            return true;
        }
        else if (v[0] >= o)
        {
            for (int i = 0; i < pos; i++)
                if (v[i] == o) return false;
        }
        size_t parent_pos = (pos + 1) / 2 - 1;
        while (pos > 0 && v[pos] > v[parent_pos]) //сортируем "веточку"
        {
            swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos = (pos + 1) / 2 - 1;
        }
        return true;
    }

    bool removeNode(const T& o) {
	size_t pos;
        for (pos = 0; pos < v.size(); pos++) //ищем элемент
            if (v[pos] == o) break;

        if (v[pos] != o) return false; //если v[pos] ! = o то такого элемента нет

        if (pos == v.size() - 1) //если o последний элемент
        { 
            v.pop_back(); 
            return true; 
        } 

        v[pos] = v[v.size() - 1];
        v.pop_back();
        
        if (v[pos] > v[(pos + 1) / 2 - 1]) //если элемент больше родителя
        {
            size_t parent_pos = (pos + 1) / 2 - 1;
            while (pos > 0 && v[pos] > v[parent_pos])
            {
                swap(v[pos], v[parent_pos]);
                pos = parent_pos;
                parent_pos = (pos + 1) / 2 - 1;
            }
        }
        else if (2 * pos + 1 < v.size())
        {
            size_t right_pos = 2 * pos + 2, left_pos = 2 * pos + 1;
            while ((left_pos < v.size() && v[pos] < v[left_pos) || (right_pos < v.size() && v[pos] < v[right_pos]))
            {
                if (v[pos] < v[left_pos])
                {
                    swap(v[pos], v[left_pos]);
                    pos = left_pos;
                    left_pos = 2 * pos + 1;
                }
                if (v[pos] < v[right_pos])
                {
                    swap(v[pos], v[right_pos]);
                    pos = right_pos;
                    right_pos = 2 * pos + 2;
                }
            }
        }
        return true;
    }

    class iterator {
        std::vector<T> *v;
        size_t idx;

    public:
        iterator() : v(nullptr) {}
        iterator(std::vector<T> *v_, size_t idx_) : v(v_), idx(idx_) {}

        ///
        /// 100
        ///  70          60
        ///  25    10    40     5
        ///   8  2  9  1  3
        ///
        /// 100 70 60 25 10 40 5 8 2 9 1 3
        ///

        iterator& operator++() {
            int n = ++idx;

        /// Если двигаемся вверх (если нет дочернего элемента справа),
        /// делим номер в дереве пополам,
        /// затем отбрасывая дробную часть, в цикле пока не дойдём до чётного элемента,
        /// затем ещё раз делим номер пополам.
        if (2 * n + 1 > v->size())
        {
            while (n % 2 != 0)
            {
                n = n / 2;
            }
            if (!(n-1))   v = nullptr;
            n = n / 2;
        }
        /// Если двигаемся вниз, то: 1) увеличиваем номер в 2 раза и прибавляем 1;
        /// 2) пока есть дочерние элементы, увеличиваем номер в 2 раза.
        else
        {
            n = 2 * n + 1;
            while (2 * n + 2 <= v->size())
            {
                while (n % 2 != 0)
                {
                    n =* 2;
                }
            }
        }
        /// Если двинуться некуда, превращаем итератор в невалидный путём зануления v.
        //
        /// Лист == номер элемента, умноженный в 2 раза, меньше количества элементов в дереве
        return *this;
        }

        iterator operator++(int) {
            auto prev = *this;
            ++(*this);
            return prev;
        }

        operator bool() const { return v != nullptr; }
        /// Теперь можно писать так:
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
        /// От корня двигаемся влево до упора
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

    HeapOverArray<int> heap;
    bool add = heap.addNode(4);
    assert(add);
    add = heap.addNode(3);
    assert(add);
    add = heap.addNode(5);
    assert(add);
    add = heap.addNode(6);
    assert(add);
    add = heap.addNode(4);
    assert(!add);
    bool remove = heap.removeNode(10);
    assert(!remove);
    remove = heap.removeNode(3);
    assert(remove);
    add = heap.addNode(14);
    assert(add);

    return 0;
}
