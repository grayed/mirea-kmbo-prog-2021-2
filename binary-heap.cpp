#include <iostream>
#include <vector>
using namespace std;

template <class T>
class HeapOverArray
{
    vector<T> v;
public:
    vector<T> getVector(){return v;}
    HeapOverArray() {}
    HeapOverArray(vector<T> a) { v = a; }
    
    bool sitdown(int ind)
    {
        while (2*ind+1<v.size())
        {
            if(ind!=0){
                int par=(ind-1)/2;
                if((2*par+2<v.size())&&v[2*par+2]<v[2*par+1]){
                    swap(v[2*par+2],v[2*par+1]);
                    ind=2*par+1;
                };
            };
            int left=2*ind+1;
            int right=2*ind+2;
            if (right>=v.size())
                right=left;
            
            if(v[ind]>=v[right])
                return true;
            swap(v[ind],v[right]);
            ind=right;
            if(v[right]<v[left]){
                swap(v[left],v[right]);
                ind=left;
            };
            
        };

    return true;
    };

    bool situp(int ind)
    {
        while(v[ind]>v[(ind-1)/2])
        {
            swap(v[ind],v[(ind-1)/2]);
            ind=(ind-1)/2;
            int left=2*ind+1;
            int right=2*ind+2;
            if(right<v.size() && v[left]>v[right])
                swap(v[left],v[right]);
        }
        int par=(ind-1)/2;
        int left=2*par+1;
        int right=2*par+2;
        if(right<v.size() && v[left]>v[right])
            swap(v[left],v[right]);
        return true;
    }

    bool addNode(const T& o)
    {
        size_t pos = v.size();
        if(pos == 0)
        {
            v.push_back(o);
            return true;
        }
        v.push_back(o);
        situp(v.size()-1);
        return true;
    }

    bool removeNode(const T& o)
    {
        int indx = 0;
        while (  indx < v.size() && v[indx] != o)
            indx++;
        if (indx == v.size())
            return false;
        v[indx] = v[v.size() - 1];
        v.pop_back();
        sitdown(indx);
        return true;
    }
};


template<class T>
class iterator
{
    vector<T>* v;
    size_t idx;
public:
    iterator() : v(nullptr) {}
    iterator(std::vector<T>* v_, size_t idx_) : v(v_), idx(idx_) {}
    size_t getindex(){ return idx;}
    
    iterator& operator++()
    {
        
        if(((idx+1)*2 + 1) > (*v).size())
        {
            while ((idx+1) % 2 != 0)
                idx = idx / 2  - 1;
            if (idx == 0)
                v = nullptr;
            idx = idx / 2;
        }
        else
        {
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

    //bool operator bool() const { return v != nullptr; }

    const T& operator*() const { return v[idx]; }
    T& operator*() { return v[idx]; }
};

template<class T>
bool testHeapIter(const vector<T>& initial, int const & indx, const vector<T>& expected)
{
    HeapOverArray<T> a(initial);
    auto v = a.getVector();
    ::iterator<T> b(&v, indx);
    for (auto i = 0; i < expected.size(); i++)
    {
        if (v[b.getindex()] != expected[i])
        {
            cerr << " difference in " << i << " expected: " << expected[i] << ", got " << v[i] << '\n';
            return false;
        }
        b++;
    }
    return true;
}

template<class T>
bool testHeapAdd(const vector<T> &initial, const T &value, const vector<T> & expected)
{
    HeapOverArray<T> a(initial);
    a.addNode(value);
    auto v = a.getVector();
    if(v.size() != expected.size())
    {
        cerr << "size difference: expected " << expected.size() << ", got " << v.size() << '\n';
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++)
    {
        if (v[i] != expected[i])
        {
            cerr << " difference in "<<i<<  " expected: " << expected[i] << ", got " << v[i] << '\n';
            return false;
        }
    }
    return true;
}

template<class T>
bool testHeapDell(const vector<T>& initial, const T& value, const vector<T>& expected)
{
    HeapOverArray<T> a(initial);
    a.removeNode(value);
    auto v = a.getVector();
    if (v.size() != expected.size())
    {
        cerr << "size difference: expected " << expected.size() << ", got " << v.size() << '\n';
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++)
    {
        if (v[i] != expected[i])
        {
            cerr << " difference in " << i << " expected: " << expected[i] << ", got " << v[i] << '\n';
            return false;
        }
    }
    return true;
}


int main()
{
    vector <int> a{ 20, 11, 15, 6,9,7,8,1,3};
    vector <int> b{ 21, 15, 20, 6,11,7,8,1,3,9};
    
    testHeapAdd(a, 21, b);
    a = {7,3,4};
    b = { 7,4,5,3 };
    testHeapAdd(a, 5, b);
    a = { 20, 11, 15, 6,9,7,8,1,3,5 };
    b = { 20, 15, 17, 6,11,7,8,1,3,5,9 };
    testHeapAdd(a, 17, b);
    
    a = { 20, 11, 15, 6,9,7,8,1,3, };
    b ={ 20, 11, 15, 6,9,7,8,1, };
    testHeapDell(a, 3, b);
    a = { 20, 11, 15, 6,9,7,8,1, };
    b = { 20, 11, 15, 1,9,7,8, };
    testHeapDell(a, 6, b);
    a = { 20, 11, 15, 1,9,7,8, };
    b = { 20, 9, 11, 1,8,7, };
    testHeapDell(a, 15, b);
    a = { 20, 9, 11, 1,7,8, };
    b = { 11, 8, 9, 1,7, };
    testHeapDell(a, 20, b);

    a = { 20, 11, 15, 6,9,7,8,1,3,5 };
    b = {1,6,3,11,5,9,20,7,15,8 };
    testHeapIter(a, 7, b);
    
    char c; cin >> c;
    return 0;
}
