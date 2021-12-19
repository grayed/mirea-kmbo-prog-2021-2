#include <iostream>
#include <string>
#include <vector>

using namespace std;


template<typename T>
class iterator {
    std::vector<T> *a;
    size_t pos;

    size_t findFirst(size_t pos) {
        while (pos % 2 == 1 && pos != 1) {
            pos /= 2;
        }
        return pos;
    }

    size_t getLeft(size_t pos) {
        while (pos * 2 <= a->size()) {
            pos *= 2;
        }
        return pos;
    }

    bool HaveRigthVert(size_t pos) {
        return (pos * 2 + 1 <= a->size());
    }

public:
    iterator() : a(nullptr) {}

    iterator(std::vector<T> *v_, size_t idx_) : a(v_), pos(idx_) {}


    iterator& operator++() {

        size_t pos = pos + 1;
        if (HaveRigthVert(pos)) {
            pos = pos * 2 + 1;
            pos = getLeft(pos);
        }
        else {
            if (pos % 2 == 1) {
                pos = findFirst(pos);
            }
            if (pos == 1) {
                pos = a->size() + 1;
                a = nullptr;
            }
            else {
                pos /= 2;
            }
        }
        pos = pos - 1;
        return *this;
    }

    operator bool()  {
        return a != nullptr;
    }

    T& operator*() {
        return (*a)[pos];
    }

    const T& operator*() const {
        return (*a)[pos];
    }

    iterator begin() {

        size_t idx = 0;
        while (idx * 2 + 1 < a->size()) {
            idx = idx * 2 + 1;
        }
        return iterator(&a, idx);
    }

    iterator end() { return iterator(); }
};





template <class T>
class Heap {
    vector<T> heap;
    int count = 0;
public:


    Heap(){}

    Heap(const vector<T>& h_) {
        for (int i = 0; i < h_.size(); ++i) {
            Insert(h_[i]);
        }
    }

    const std::vector<T>& getVector()  { return heap; }

    void SiftDown(int pos) {
        while (2 * pos + 1 < count) {
            int left = 2 * pos + 1;
            int right = 2 * pos + 2;
            int j = -1;

            if (right < count) {
                if (heap[right] > heap[left]) {
                    j = right;
                } else if (heap[left] > heap[pos]) {
                    j = left;
                }
            } else if (heap[left] > heap[pos]) {
                j = left;
            }
            if (j != -1 && heap[pos] < heap[j]) {
                swap(heap[pos], heap[j]);
                pos = j;
            } else
                break;
        }
    }

    void SiftUp(int pos) {
        while ((pos - 1) / 2 >= 0 && heap[(pos - 1) / 2] < heap[pos]) {
            swap(heap[pos], heap[(pos - 1) / 2]);
            pos = (pos - 1) / 2;
        }
    }

    void Insert(int numb) {
        count++;
        heap.push_back(numb);
        SiftUp(count - 1);
    }


    T Erase(int numb) {
        int pos = -1;
        for (int i = 0; i < count; ++i) {
            if (heap[i] == numb) {
                pos = i;
                break;
            }
        }
        if (pos == -1) return 0;

        T answ = heap[pos];
        heap[pos] = heap[count - 1];
        heap.pop_back();
        count--;

        //int p = (pos - 1) / 2;
        /*if (heap[pos] > heap[p]) //никогда не поднимаемся?
            SiftUp(pos);
        else*/
        SiftDown(pos);
        return answ;
    }


    T RemoveMax() {
        T answ = heap[0];
        heap[0] = heap[count - 1];
        count--;
        SiftDown(0);
        return answ;
    }

    void print() {
        for (int i = 0; i < count; ++i)
            cout << heap[i] << " ";
        cout << endl;
    }

};

template<typename T>
bool testIterator(const vector<T>& init, const vector<T>& expected) {
    Heap<T> heap(init);
    vector<T> v;
    for (auto it = heap.begin(); it; it++) {
        v.push_back(*it);
    }
    if (v.size() != expected.size()) {
        cout << "size if not equal " << expected.size() << " , " << v.size() << endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++)
        for (size_t i = 0; i < expected.size(); i++) {
            if (v[i] != expected[i]) {
                cout << "difference: pos " << i << ": expected val " << expected[i] << ", got val " << v[i] << endl;
                return false;
            }
        }
    return true;
}

template<typename T>
bool testRemove(const vector<T>& initial,  const vector<T>& expected, T val)
{

    Heap<T> heap(initial);

    heap.Erase(val);
    auto v = heap.getVector();
    if (v.size() != expected.size()) {
        cout << "size is not equal: expected sz " << expected.size() << ", got sz " << v.size() << endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++) {
        if (v[i] != expected[i]) {
            cout << "difference: pos " << i << ": expected val " << expected[i] << ", got " << v[i] << endl;
            return false;
        }
    }
    return true;
}

int main() {

    vector<int> v = {2, 3, 13, 4, 6 , 7, 8};
    vector<int> exp = {8, 6, 7, 2, 4, 3};
    testRemove<int>(v, exp, 13);
    /*Heap<int> h(v);
    h.print();
    h.Insert(8);
    h.print();
    h.RemoveMax();
    h.print();*/

    Heap<int> h1;
    h1.Insert(5);
    h1.Insert(6);
    h1.Insert(7);
    h1.Insert(4);
    h1.Insert(3);
    h1.Erase(6);
    h1.print();

    return 0;
}


