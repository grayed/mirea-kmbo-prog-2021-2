#include <iostream>
#include <vector>
#include <algorithm>

template<class T>
class HeapOverArray
{
    std::vector<T> v;
public:
    HeapOverArray() {}

    const std::vector<T>& getVector() const { return v; }

    // true, если такого элемента нет, иначе false
    // в моей реализации такой проверки нет, так что
    // пока никакой проверки нет, так что будем надеяться на умного пользователя,
    // который вносит только разные значения
    bool addNode(const T& o)
    {
        // 1. Определем место вставки (первое свободное место в горизонтальном ряду)
        // 2. Сравниваем с родителем, если значение больше родительского, меняем его местами с родителем (в цикле).

        size_t pos = v.size();
        v.push_back(o);
        if (pos == 0)
        {
            // дерево было пустое
            return true;
        }
        size_t parent_pos = (pos + 1) / 2 - 1;
        // я так понимаю мы получили, что pos соответсвтует последнему элементу в массиве
        // далее берем + 1 тк нужен номер элемента, далее делим на 2 и получаем номер родителя
        // далее вычитаем 1 чтобы получить какому индексу в массиве соответствует родитель
        while (parent_pos >= 0 && v[pos] > v[parent_pos])
        {
            std::swap(v[pos], v[parent_pos]);
            // v[pose] это наш элемент o

            pos = parent_pos;
            parent_pos = (pos + 1) / 2 - 1;
        }
        // закомментил тк это не надо
        // if (pos == 0) return true; // мы в корне
        // if (pos == v.size() - 1)  return true; // даже если мы слева, то справа ничего нет
        // if (pos % 2 == 1) return true; // мы справа
        // // осталось что мы слева и не в конце, значит право есть
        // if (v[pos] > v[pos + 1]) std::swap(v[pos, v[pos + 1]]);
        return true;
    }

    bool removeNode(const T& o)
    {
        // найдем этот элемент и его индекс
        size_t index = 0;
        while (index < v.size() && v[index] != o)
            index++;
        
        if (index == v.size())
        {
            // случай где такого элемента просто нет
            return false;
        }
        v[index] = v[v.size() - 1];
        v.pop_back(); // удалили элемент

        // теперь надо этот элемент поставить куда надо
        // сначала посмотрим, если он больше родителя
        size_t pos = index + 1, parent_pos = pos / 2;
        while (parent_pos > 0 && v[pos - 1] > v[parent_pos - 1])
        {
            std::swap(v[pos - 1], v[parent_pos - 1]);
            
            // мы свапаем с родителем
            // если мы слева, то ничего делать не надо
            // если мы справа, то надо свапнуть правый и левый
            if (pos % 2 == 0 && pos != v.size() && ) std::swap(v[pos - 1], v[pos]);

            pos = parent_pos;
            parent_pos = pos / 2;
        }
        // теперь аналогично с ребенком
        size_t child_pos = pos * 2;
        while (child_pos <= v.size() && v[pos - 1] < v[child_pos - 1])
        {

        }
        return true;
    }
    class iterator
    {
        std::vector<T> *v;
        size_t idx;

    public:
        iterator : v(nullptr) {}
        iterator(std::vector<T> *v_, size_t idx_) : v(v_), idx(idx_) {}

        iterator& operator++()
        {
            /// 1. узнаем высоту элемента
            /// 2. узнаем, слева он или справа

            /// если двигаемся вверх (если нет дочернего элемента справа),
            /// делим номер в дереве пополам.
            /// затем отбрасывая дробную часть, в цикле пока не дойдем до четного элемента
            /// затем еще раз делим номер пополам
            ///
            /// если двигаемся вниз, то: 1) увеличиваем номер в 2 раза и прибавляем 1
            /// 2) пока есть дечерние элементы, увеличиваем номер в 2 раза
            ///
            /// если двигаться некуда, превращаем итератор в невалидный путем зануления v
            ///
            /// лист == номер элемента, умноженный в 2 раза, меньше количесвта элементов в дереве




            return *this;
        }

        iterator operator++(int)
        {
            auto prev = *this;
            ++(*this);
            return prev;
        }

        bool operator bool() const { return v != nullptr; }
        
        const T& operator*() const { return v[idx]; }
        T& operator*() { return v[idx]; }
    }
};

template<class T>
bool testHeapAdd(const std::vector<T> &initial, const T &value, const std::vector<T> &expected)
{
    HeapOverArray<T> heap(initial);
    heap.addNode(value);
    auto v = heap.getVector();
    if (v.size() != expected.size())
    {
        std::cerr << "size difference^ expected " << expected.size() << ", got " << v.size() << std::endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++)
        if (v[i] != expected[i])
        {
            std::cerr << "differencein " << i << ": expected " << expected[i] << ", got " << v[i] << std::endl;
            return false;
        }
    return true;
}

int main()
{
    std::vector<int> initial = { 7, 3, 4 };
    std::vector<int> expected = { 8, 4, 7, 3 };
    testHeapAdd(initial, 8, expected);
    system("pause");
    return 0;
}

int main()
{
    vector<int> initial = { 7, 3, 4 };
    vector<int> expected = { 8, 4, 7, 3 };
    testHeapAdd(initial, 8, expected);

    /// TODO тесты для остальных операций с кучей

    return 0;
}
