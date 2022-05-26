
#include <iostream>
#include <vector>
#include <cassert>

template<class T>
class HeapOverArray {
    std::vector<T> v;

public:
    HeapOverArray() {}
    HeapOverArray(const std::vector<T>& initv) : v(initv) {}

    const std::vector<T>& getVector() const { return v; }
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

        size_t findLeftmost(size_t pos) {
            while (pos * 2 <= v->size()) {
                pos *= 2;
            }
            return pos;
        }

        size_t findFirstEvenAncestor(size_t pos) {
            while (pos % 2 == 1 && pos != 1) {
                pos /= 2;
            }
            return pos;
        }

        bool hasRight(size_t pos) {
            return (pos * 2 + 1 <= v->size());
        }

    public:
        iterator() : v(nullptr) {}
        iterator(std::vector<T>* v_, size_t idx_) : v(v_), idx(idx_) {}

        iterator& operator++() {
            size_t pos = idx + 1;
            if (hasRight(pos)) {
                pos = pos * 2 + 1;
                pos = findLeftmost(pos);
            }
            else {
                if (pos % 2 == 1) {
                    pos = findFirstEvenAncestor(pos);
                }
                if (pos == 1) {
                    pos = v->size() + 1;
                    v = nullptr;
                }
                else {
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

        const T& operator*() const { return (*v)[idx]; }
        T& operator*() { return (*v)[idx]; }
    };

    iterator begin() {
        if (v.empty()) {
            return iterator();
        }
        
    }

    iterator end() {
        return iterator();
    }
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


int main() {
    std::vector<int> initial = {7, 3, 4};
    std::vector<int> expected = {8, 7, 4, 3};
    testHeapAdd(initial, 8, expected);
    
    return 0;
}
