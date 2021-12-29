#include <iostream>
#include <vector>

using namespace std;

template<class T>
class HeapOverArray {
    std::vector<T> v;

public:
    HeapOverArray() {}
    HeapOverArray(const std::vector<T> &initv) : v(initv) {}

    const std::vector<T>& getVector() const { return v; }

    bool addNode(const T& o) {
        size_t pos = v.size();
        if(pos == 0) {
            v.push_back(o);
            return true;
        }
        int parent_pos = (pos + 1) / 2 - 1;
        v.push_back(o);
        while ( parent_pos>=0 &&  o > v[parent_pos] ) {
            swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos = ((pos + 1) / 2) - 1;
        }
        if (pos == 0) { 
            if(v[1]>v[2])
                swap(v[1], v[2]);
        }
        else {
            if (v[(parent_pos + 1) * 2-1] > v[(parent_pos + 1) * 2])
                swap(v[(parent_pos + 1) * 2], v[(parent_pos + 1) * 2 - 1]);
        }
        return true;
    }

    bool removeNode(const T& o) {
	int indx = 0;
        while (  indx < v.size() && v[indx] != o)
            indx++;
        if (indx == v.size())
            return false;
        v[indx] = v[v.size() - 1];
        v.pop_back();
        if (indx == v.size())
            return true;
        if(indx > 0  && v[indx] > v[(indx + 1) / 2 - 1]) {
            while (indx > 0 &&  v[indx] > v[(indx + 1) / 2 - 1]) {
                swap(v[indx], v[(indx + 1) / 2 - 1]);
                indx = (indx + 1) / 2 - 1;
            }
            if (indx == 0)
                return true;
            if (v[indx]>v[((indx + 1) / 2 - 1)*2+1]) { 
                swap(v[indx], v[((indx + 1) / 2 - 1) * 2 + 1]);
            }
            return true;
        }
        else {
            while (indx * 2 + 2 < v.size()) {
                if ((v[indx] < v[indx * 2 + 2]) && (v[indx] < v[indx * 2 + 1])) {
                    if (v[indx * 2 + 2] > v[indx * 2 + 1]) {
                        swap(v[indx], v[indx * 2 + 2]);
                        indx = indx * 2 + 2;
                    }
                    else {
                        swap(v[indx], v[indx * 2 + 1]);
                        indx = indx * 2 + 1;
                    }
                }
                else if (v[indx] < v[indx * 2 + 2]) {
                    swap(v[indx], v[indx * 2 + 2]);
                    indx = indx * 2 + 2;
                }
                else if (v[indx] < v[indx * 2 + 1]) {
                    swap(v[indx], v[indx * 2 + 1]);
                    indx = indx * 2 + 1;
                }
                else
                    return true;

            }
            if (indx * 2 + 1 < v.size() && v[indx] < v[indx * 2 + 1])
                swap(v[indx], v[indx * 2 + 1]);
            return true;
        }
    }
};

template<class T>
class iterator {
    vector<T>* v;
    size_t idx;
public:
    iterator() : v(nullptr) {}
    iterator(std::vector<T>* v_, size_t idx_) : v(v_), idx(idx_) {}
    size_t getindex(){ return idx;}

    iterator& operator++() {    
        if(((idx+1)*2 + 1) > (*v).size()) {
            while ((idx+1) % 2 != 0) {
                idx = idx / 2  - 1;
            }
            if (idx == 0)
                v = nullptr;
            idx = idx / 2;
        }
        else {
            idx = idx * 2 + 2;
            while (idx * 2 + 1 < (*v).size())
                idx = idx * 2 + 1;
        }
        return *this;
    }

    iterator operator++(int) {
        auto prev = *this;
        ++(*this);
        return prev;
    }
	
	const T& operator*() const { return v[idx]; }
    T& operator*() { return v[idx]; }
};

template<class T>
bool testHeapAdd(const vector<T> &initial, const T &value, const vector<T> & expected) {
    HeapOverArray<T> a(initial);
    a.addNode(value);
    auto v = a.getVector();
    if(v.size() != expected.size()) {
        cerr << "size difference: expected " << expected.size() << ", got" << v.size() << '\n';
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++) {
        if (v[i] != expected[i]) {
            cerr << " difference in "<<i<<  " expected: " << expected[i] << ", got" << v[i] << '\n';
            return false;
        }
    }
    return true;
}

template<class T>
bool testHeapRemove(const vector<T>& initial, const T& value, const vector<T>& expected) {
    HeapOverArray<T> a(initial);
    a.removeNode(value);
    auto v = a.getVector();
    if (v.size() != expected.size()) {
        cerr << "size difference: expected " << expected.size() << ", got" << v.size() << '\n';
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++) {
        if (v[i] != expected[i]) {
            cerr << " difference in " << i << " expected: " << expected[i] << ", got" << v[i] << '\n';
            return false;
        }
    }
    return true;
}

int main()
{
    vector <int> init{ 20, 11, 15, 6,9,7,8,1,3, };
    vector <int> exp{ 21, 15, 20, 6,11,7,8,1,3,9};
    testHeapAdd(init, 21, exp);
    init = {7,3,4};
    exp = { 7,4,5,3 };
    testHeapAdd(init, 5, exp);
	
    init = { 20, 11, 15, 6,9,7,8,1,3,5 };
    exp = { 20, 15, 17, 6,11,7,8,1,3,5,9 };
    testHeapRemove(init, 3, exp);
    init = b;
    exp = { 20, 11, 15, 1,9,7,8, };
    testHeapRemove(init, 6, exp);

    return 0;
}
