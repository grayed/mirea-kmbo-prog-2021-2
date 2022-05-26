#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


template<class T>
class HeapOverArray {
    std::vector<T> v;

public:
    HeapOverArray() {}
    HeapOverArray(const std::vector<T>& initv) : v(initv) {}    /// Требуется, чтобы массив был заранее упорядочен

    const std::vector<T>& getVector() const { return v; }

    /// Если такой элемент уже существует, то изменения не вносятся, а функция возвращает false.
    /// Если элемент добавлен, то функция возвращает true.
       

        bool addNode(const T & o) {
            /// 1. Определяем место вставки (первое свободное место в нижнем горизонтальном ряду).
       /// 2. Сравниваем с родителем, если значение больше родительского, меняем местами с родителем (в цикле).
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

    /// ЗАДАЧИ, ч.2
    ///  1. Удаление элемента из двоичной кучи
    ///  2. Тестирование корректности реализации
    ///  3. Построение итератора для двоичной кучи
    ///

    bool removeNode(const T& o) 
    {
        auto it = std::find(v.begin(), v.end(), o); //возвращаем итератор на нужный элемент

        if (it != o) { //такого элемента нет
            return false;
        }
        else if (it != v.end()) //удаляем не последний элемент
        {
            //просто меняем наш элемент с последним местами и удаляем
            std::swap(it, v.rbegin());
            v.pop_back();
            v.shrink_to_fit(); //обрезали массив

            //восстанавливаем порядок ??

            
        }
        else //последний элемент
        {
            v.pop_back();
            v.shrink_to_fit(); //очищаем неиспользованное пространство
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
            else {
                size = 2 * size + 1;
                while (2 * size + 2 <= v->size()) {
                    while (size % 2 != 0) {
                        size = *2;
                    }
                }
            }
           /// 1. Узнаём высоту элемента
            /// 2. Узнаём, слева мы или справа

            /// Если двигаемся вверх (если нет дочернего элемента справа),
            /// делим номер в дереве пополам,
            /// затем отбрасывая дробную часть, в цикле пока не дойдём до чётного элемента,
            /// затем ещё раз делим номер пополам.
            ///
            /// Если двигаемся вниз, то: 1) увеличиваем номер в 2 раза и прибавляем 1;
            /// 2) пока есть дочерние элементы, увеличиваем номер в 2 раза.
            ///
            /// Если двинуться некуда, превращаем итератор в невалидный путём зануления v.
            ///

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
