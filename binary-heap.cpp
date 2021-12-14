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
        if (pos == 0) return true; // мы в корне
        if (pos == v.size() - 1)  return true; // даже если мы слева, то справа ничего нет
        if (pos % 2 == 1) return true; // мы справа
        // осталось что мы слева и не в конце, значит право есть
        if (v[pos] > v[pos + 1]) {
            //swap(v[pos, v[pos + 1]]);
            auto temp = v[pos];
            v[pos] = v[pos + 1];
            v[pos + 1] = temp;
        }
    
        /// TODO
	return true;
    }

    bool removeNode(const T& o) {
	/// TODO

        size_t pos = 0;
        if (v.size() == 0) // пустое дерево
            return false;
        while (pos < v.size() && v[pos] != o )
            ++pos;
        if (pos = v.size())             //не нашли элемент
            return false;
        v.pop_back();
        if (pos == v.size() - 1) {      //удаляем последний
            return true;
        }
        size_t parent_pos = (pos - 1) / 2;
        v[i] = o;
        while (v[parent_pos] < v[pos]) {    //поднимаемся по дереву
            swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos= (pos - 1) / 2;
        }
        if (pos == 0)
            return true;
        while (2*pos < v.size() && (v[2 * pos] > o || v[2*pos+1] > o )) { //спускаемся вниз
            if (v[2 * pos] > o && v[2 * pos + 1] < o) {// больше только левый
                swap(v[2 * pos], v[pos]);
                pos = pos * 2;
            }
            else if (v[2 * pos] > o && v[2 * pos + 1] > o) { // оба больше
                if (v[2 * pos + 1] > v[2 * pos]) {
                    swap(v[2 * pos + 1], v[pos]);
                    pos = 2 * pos + 1;
                }
                else {
                    swap(v[pos], v[2 * pos]);
                    pos = 2 * pos;
                }
            }
            else {  //больше только правый
                swap(v[2 * pos + 1], v[pos]);
                pos = 2 * pos + 1;
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
            /// Лист IFF номер элемента, умноженный в 2 раза, меньше количества элементов в дереве

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
bool testHeapAdd(const std::vector<T>& initial, const T& value,
    const std::vector<T>& expected)
{
    HeapOverArray<T> heap(initial);
    heap.addNode(value);
    auto v = heap.getVector();
    if (v.size() != expected.size()) {
        std::cerr << "size difference: expected " << expected.size() <<
            ", got " << v.size() << std::endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++) {
        if (v[i] != expected[i]) {
            std::cerr << "diffecrence in " << i << ": expected " <<
                expected[i] << ", got " << v[i] << std::endl;
            return false;
        }
    }
    return true;
}

template<class T>
bool testHeapIterator(const std::vector<T>& initial,
    const std::vector<T>& expected)
{
    HeapOverArray<T> heap(initial);
    std::vector<T> v;
    for (auto it = heap.begin(); it; it++) {
        v.push_back(*it);
    }
    if (v.size() != expected.size()) {
        std::cerr << "size difference: expected " << expected.size() <<
            ", got " << v.size() << std::endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++) {
        if (v[i] != expected[i]) {
            std::cerr << "diffecrence in " << i << ": expected " <<
                expected[i] << ", got " << v[i] << std::endl;
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

    /// TODO тесты для остальных операций с кучей

    return 0;
}
