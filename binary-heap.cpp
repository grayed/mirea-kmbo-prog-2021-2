#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

template <class T>
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
        int n = ++idx;

        if (2 * n + 1 > v->size()) {
            while (n % 2 != 0) n = n / 2;
            if (!(n - 1)) v = nullptr;
            n /= 2;
        }
        else {
            n = 2 * n + 1;
            while (2 * n + 2 <= v->size()) {
                while (!(n % 2)) n *= 2;
            }
        }

        return *this;
    }

    iterator operator++(int) {
        auto prev = *this;
        ++(*this);
        return prev;
    }

    //bool operator bool() const { return v != nullptr; }

    const T& operator*() const { return v[idx]; }
    T& operator*() { return v[idx]; }
};

class Node {
    Node* left, * right, * parent;
    friend class Tree;

public:
    Node* getLeft() { return left; }
    const Node* getLeft() const { return left; }
    Node* getRight() { return right; }
    const Node* getRight() const { return right; }
    Node* getParent() { return parent; }
    const Node* getParent() const { return parent; }

    std::string name;
    std::string description;
    //long price;

    Node(const std::string& name_, Node* parent_ = nullptr)
        : left(nullptr), right(nullptr), parent(parent_), name(name_)
    { }

    Node* findMin() {
        Node* node = this;
        while (node->left)
            node = node->left;
        return node;
    }

    Node* findMax() {
        Node* node = this;
        while (node->right)
            node = node->right;
        return node;
    }
};

template <class T>
class HeapOverArray {
	std::vector<T> v;

public:
	HeapOverArray() {}

	bool addNode(const T& o) {

        size_t pos = v.size();
        v.push_back(o);
        if (pos == 0) return true;
        else if (v[0] >= o)
            for (int i = 0; i < pos; i++)
                if (v[i] == o) return false;

        size_t parent_pos = (pos + 1) / 2 - 1;
        while (pos > 0 && v[pos] > v[parent_pos]) {
            swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos = (pos + 1) / 2 - 1;
        }
        return true;

	}

	bool removeNode(const T & val) {
        size_t pos = -1;
        for (int i = 0; i < v.size(); ++i)
            if (v[i] == val) {
                pos = i;
                break;
            }

        if (pos == -1)return false;

        v[pos] = v[v.size() - 1];
        v.pop_back();

        while (pos > 0 && v[pos] > v[(pos + 1) / 2 - 1]) {
            swap(v[pos], v[(pos + 1) / 2 - 1]);
            pos = (pos + 1) / 2 - 1;
        }

        if (pos == 0) return true;

        while (pos * 2 + 1 < v.size() && v[pos] < v[2 * pos] && v[pos] < v[pos * 2 + 1]) {
            swap(v[2 * pos], v[pos]);
            pos *= 2;
        }
        if (pos * 2 < v.size() && v[pos] < v[pos * 2])
            swap(v[2 * pos], v[pos]);
        return true;
	}
};

int main() {
    HeapOverArray <int> heap;
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
    cout << "The code is working just fine";
}
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

    
