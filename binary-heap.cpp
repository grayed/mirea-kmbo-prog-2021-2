#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


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
        /// 1. Определяем место вставки (первое свободное место в нижнем горизонтальном ряду).
        /// 2. Сравниваем с родителем, если значение больше родительского, меняем местами с родителем (в цикле).

        size_t pos = v.size(); //v-вектор вершин
        if (pos == 0) {

            v.push_back(o);
            return true;
        }
        if (std::find(v.begin(), v.end(),o) !=v.end()) //если элемент уже добавлен
                return false;
 
        size_t parent_pos = (pos + 1) / 2 - 1; //если добавляем самый первый элемент,то получим индекс -1
        //в parent_pos хранится индекс элемента,который может стать родителем
       
        v.push_back(o); //всегда добавление в конец
         
        //тут проверяем корректность структуры
        //i-индекс родителя
        //2*i+1-индекс левого потомка
        //2*i+2-индекс правого потомка
        int i=pos-1;//последний индекс,наш добавленный элемент
        while(i>0 && v[parent_pos]<v[i])//пока не дошли до корня и пока ребенок больше родителя
        {
            T temp=v[i];
            v[i]=v[parent_pos];
            v[parent_pos]=temp;

            i=parent_pos;
            parent_pos=(i+1)/2-1; //новый родитель
        }

        return true;
        
    }
   

    bool removeNode(const T& o) 
    {
        if (!std::find(v.begin(), v.end(),o)==v.end()) //если элемент найден в векторе
            return false;
        // size_t Беззнаковое целое. То есть, это не int, он знаковый.
        //Предназначен для отображения размера любого объекта в байтах:
        //это возвращает size_of и много функций стандартной библиотеки - всякие length и count.
            
        //vector<int>::iterator it;
        //it-итератор
        //vector<int>::iterator-тип данных итератора
        //итератор от вектора, вектор типа int

        vector<int>::iterator it=std::find(v.begin(),v.end(),o);
        int rem_ind;
        if(it!=v.end()) //v.end() указывает на пустой участок после последнего элемента
            int rem_ind=std::distance(v.begin(),it); //v.begin() указатель на первый элемент, содержит адрес первого элемента,
            //для получения значения надо разыменовать 
            
        if (rem_ind==0)
        {
            v.erase(rem_ind);
            return true;
        }
            
        int new_ind=v.size()-1; //последний элемент, который берем 

        v[rem_ind]=v[new_ind];
        v.erase(new_ind); //erase удаляет элемент из коллекции,на который указывает итератор

           
        //сначала проверить не больше ли он родительского 
        
        size_t parent_pos = (rem_ind + 1) / 2 - 1; 
        T temp;
        int current_ind=rem_ind;
        if (v[current_ind]>v[parent_pos])
            while(parent_pos>=0 && v[current_ind]>v[parent_pos] )
            {
                temp=v[current_ind];
                v[current_ind]=v[parent_pos];
                v[parent_pos]=temp;
                //так как все дочерние элементы меньше узла,то для дочерних удаляемого ничего не нарушено, поскольку они меньше родителя удаляемого элемента

                current_ind=parent_pos;
                parent_pos = (current_ind + 1) / 2 - 1;

            }
            
        //либо узел уже сместился,либо смещение не требовалось

        //удаляемый не больше родителя
        //проверяем,что оба дочерних меньше
    
       if(2*current_ind+1!=0 && 2*current_ind+2!=0 && v[current_ind]>v[2*current_ind+1] && 
        v[current_ind]>v[2*current_ind+2]) //есть оба дочерних и они оба меньше
            return true; //удаление произошло

        //узел и у него два дочерних элемента и один берется для его замены при его удалении,останется один
        //возможно только для дочерних-листов
        if(2*current_ind+1!=0 && 2*current_ind+2==0 && v[current_ind]>v[2*current_ind+1]) //есть один дочерний и он меньше (для  листа,у которого и берется дочерний для его замены)
            return true; //удаление произошло
        //или такое возможно при удалении главной вершины, у которой всего два потомка
            
        if (2*current_ind+2<v.size() && 2*current_ind+1<v.size()) //изначально было два дочерних
        {
            //если же меньше дочерних,то теперь корректируется это
            while(v[current_ind]<v[2*current_ind+2] || v[current_ind]<v[2*current_ind+1] )
            {
                int index_=(v[2*current_ind+1]>=v[2*current_ind+2])? 2*current_ind+1 : 2*current_ind+2;
                T temp_=v[index_];
                v[index_]=v[current_ind];
                v[current_ind]=temp;

                current_ind=index_;

                if(2*current_ind+2>=v.size() && 2*current_ind+1>=v.size()) //дошли до поcледней вершины
                    return true;

            }

            return true;
            
        }

        if((2*current_ind+2<v.size() && 2*current_ind+1>=v.size()) || (2*current_ind+1<v.size() && 2*current_ind+2>=v.size())) // если есть один дочерний (лист) 
        {
                
            int index_=(v[2*current_ind+1]>=v[2*current_ind+2])? 2*current_ind+1 : 2*current_ind+2;
            T temp_=v[index_];
            v[index_]=v[current_ind];
            v[current_ind]=temp;
                
            return true;

        }

    } 
    
    class iterator 
    {
        std::vector<T> *v;
        size_t idx;
        
    public:
        iterator() : v(nullptr) {}
        iterator(std::vector<T> *v_, size_t idx_) : v(v_), idx(idx_) {}
        
        
        iterator& operator++() 
        {   
            int index;
        //Адрес, хранящийся в переменной v, является адресом первого элемента массива.
        
            if (2*idx+2<v->size()) //если есть справа
            {   
                idx=2*idx+2;
                index=idx;
                while(2*idx+1<v->size()) //пока есть что-то слева
                {
                    idx=2*idx+1;
                    index=idx;
                
                }

            }

            else
            {
                if(idx==0 && 2*idx+1>=v->size() && 2*idx+2>=v->size()) //всего один элемент
                {
                    v=nullptr;
                    return *this;
                }

                while(idx % 2==0)    //пока в правом поддереве (в правом поддереве четные индексы)
                {
                    idx=(idx-2)/2;
                    index=idx;

                    if (idx==0) //если справа пришли в корень
                    {
                        v=nullptr;
                        return *this;
                    }

                }

                idx=(idx + 1) / 2 - 1; //родитель

                index=idx;
            }



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

        int index=0;
        while(index<=v.size()-1) //пока индекс допустим
            index=2*index+1;

        return iterator(v,index); //итератор на первый элемент (самый левый нижний)
        
        
        
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
    return 0;
}
