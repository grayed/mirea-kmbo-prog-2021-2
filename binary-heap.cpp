#include <iostream>
#include <string>
#include <vector>

using namespace std;

///TODO : юнит-тесты, итератор

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

int main() {

    vector<int> v = {2, 3, 13, 4, 6 , 7};
    Heap<int> h(v);
    h.print();
    h.Insert(8);
    h.print();
    h.RemoveMax();
    h.print();

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


