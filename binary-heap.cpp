#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


template<class T>
class HeapOverArray {
    std::vector<T> v;

    //ñêðûòûé èíòåðôåéñ
    void HeapMaxify(size_t ind, size_t size = v.size())
    {

        //https://habr.com/ru/post/587228/

        //îïðåäåëÿåì ëåâûé è ïðàâûé äî÷åðíèè óçëû
        size_t  left   = 2 * ind
                ,right = left + 1;

        //ìàêñèìàëüíîå, êàê ìû äóìàåì, çíà÷åíèå
        size_t largest = ind;

        //åñëè ëåâûé äî÷åðíèé óçåë áîëüøå ÷åì êîðåíü
        //ïîääåðåâà(êîðåíü ñ èíäåêñîì largest)
        //òî ìû èçìåíÿåì largest íà èíäåêñ ëåâîãî
        //èëè ïðàâîãî äî÷åðíåãî óçëà
        if (left < size && v[left] > v[largest]) {
            largest = left;
        }
        if (right < size && v[right] > v[largest]) {
            largest = right;
        }

        //åñëè âñ¸-òàêè çíà÷åíèå èçíà÷àëüíî ïðîâåðÿåìîãî 
        //óçëà ñ èíäåêñîì i íå ÿâëÿåòñÿ ñàìûì áîëüøèì
        //òî ïðîâåðÿåìûé óçåë è óçåë ñ áîëüøèì çíà÷åíèåì ìåíÿþòñÿ ìåñòàìè
        if (largest != ind) {
            std::swap(v, ind, largest);

            //Ðåêóðñèâíî âûçûâàåì ôóíêöèþ, ÷òîáû â ñëó÷àå ÷åãî
            //îòïðàâèòü ýëåìåíò íèæå ïî ïèðàìèäå
            HeapMaxify(largest, size);
        }
    }

public:
    HeapOverArray() {}
    HeapOverArray(const std::vector<T>& initv) : v(initv) {}    /// Òðåáóåòñÿ, ÷òîáû ìàññèâ áûë çàðàíåå óïîðÿäî÷åí

    const std::vector<T>& getVector() const { return v; }

    /// Åñëè òàêîé ýëåìåíò óæå ñóùåñòâóåò, òî èçìåíåíèÿ íå âíîñÿòñÿ, à ôóíêöèÿ âîçâðàùàåò false.
    /// Åñëè ýëåìåíò äîáàâëåí, òî ôóíêöèÿ âîçâðàùàåò true.
       

        bool addNode(const T & o) {
            /// 1. Îïðåäåëÿåì ìåñòî âñòàâêè (ïåðâîå ñâîáîäíîå ìåñòî â íèæíåì ãîðèçîíòàëüíîì ðÿäó).
       /// 2. Ñðàâíèâàåì ñ ðîäèòåëåì, åñëè çíà÷åíèå áîëüøå ðîäèòåëüñêîãî, ìåíÿåì ìåñòàìè ñ ðîäèòåëåì (â öèêëå).
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

    /// ÇÀÄÀ×È, ÷.2
    ///  1. Óäàëåíèå ýëåìåíòà èç äâîè÷íîé êó÷è
    ///  2. Òåñòèðîâàíèå êîððåêòíîñòè ðåàëèçàöèè
    ///  3. Ïîñòðîåíèå èòåðàòîðà äëÿ äâîè÷íîé êó÷è
    ///

    bool removeNode(const T& o) 
    {
        auto it = std::find(v.begin(), v.end(), o); //âîçâðàùàåì èòåðàòîð íà íóæíûé ýëåìåíò

        if (it != o) { //òàêîãî ýëåìåíòà íåò
            return false;
        }
        else if (it != v.end()) //óäàëÿåì íå ïîñëåäíèé ýëåìåíò
        {
            //ïðîñòî ìåíÿåì íàø ýëåìåíò ñ ïîñëåäíèì ìåñòàìè è óäàëÿåì
            std::swap(it, v.rbegin());
            v.pop_back();
            v.shrink_to_fit(); //îáðåçàëè ìàññèâ

            //âîññòàíàâëèâàåì ïîðÿäîê ??

            
        }
        else //ïîñëåäíèé ýëåìåíò
        {
            v.pop_back();
            v.shrink_to_fit(); //î÷èùàåì íåèñïîëüçîâàííîå ïðîñòðàíñòâî
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
            /// 1. Óçíà¸ì âûñîòó ýëåìåíòà
            /// 2. Óçíà¸ì, ñëåâà ìû èëè ñïðàâà

            /// Åñëè äâèãàåìñÿ ââåðõ (åñëè íåò äî÷åðíåãî ýëåìåíòà ñïðàâà),
            /// äåëèì íîìåð â äåðåâå ïîïîëàì,
            /// çàòåì îòáðàñûâàÿ äðîáíóþ ÷àñòü, â öèêëå ïîêà íå äîéä¸ì äî ÷¸òíîãî ýëåìåíòà,
            /// çàòåì åù¸ ðàç äåëèì íîìåð ïîïîëàì.
            ///
            /// Åñëè äâèãàåìñÿ âíèç, òî: 1) óâåëè÷èâàåì íîìåð â 2 ðàçà è ïðèáàâëÿåì 1;
            /// 2) ïîêà åñòü äî÷åðíèå ýëåìåíòû, óâåëè÷èâàåì íîìåð â 2 ðàçà.
            ///
            /// Åñëè äâèíóòüñÿ íåêóäà, ïðåâðàùàåì èòåðàòîð â íåâàëèäíûé ïóò¸ì çàíóëåíèÿ v.
            ///
            /// Ëèñò == íîìåð ýëåìåíòà, óìíîæåííûé â 2 ðàçà, ìåíüøå êîëè÷åñòâà ýëåìåíòîâ â äåðåâå

            return *this;
        }

        iterator operator++(int) {
            auto prev = *this;
            ++(*this);
            return prev;
        }

        operator bool() const { return v != nullptr; }
        /// Òåïåðü ìîæíî ïèñàòü òàê:
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
        /// Îò êîðíÿ äâèãàåìñÿ âëåâî äî óïîðà
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
