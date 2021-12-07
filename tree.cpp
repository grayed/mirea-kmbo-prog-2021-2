#include <cassert>
#include <iostream>
#include <string>

///
/// Домашнее задание:
///
/// 1.  Добавить findMin() и findMax() в классе Node, аналогичные таковым в Tree;
///     они должны искать в поддереве, корнем которого является текущий узел. +++
///
/// 2.  Реализовать малый правый поворот, а также большие левый и правый повороты
///     в классе Tree, по аналогии с малым левым поворотом. +++
///
/// 3.  Реализовать префиксный оператор «++» для итератора. +++
///
/// 4.  Реализовать Tree::findNearest() и Tree::deleteNode().
///
/// 5.  Реализовать юнит-тесты на все публичные методы классов Tree и TreeIterator,
///     кроме тривиальных. Прогонять эти тесты в функции main().
///

/**
1. Добавление.
    а) (интерфейсный способ) Создаём дерево, вызываем addNode(), проверяем:
        * что элемент добавился?
        * что элемент добавился по соседству с определёнными другими элементами?
        * сравнить дерево целиком с эталоном?
    б) (инвазивный) Конструируем объекты Node и Tree вручную, затем вызываем addNode(), проверяем (то же).
2. Удаление.
    а) Создаём дерево, вызываем removeNode(), проверяем:
        * что элемента в дереве больше нет?
        * что бывшие соседи элемента получили определённое новое состояние?
        * сравнить дерево целиком с эталоном?
    б) Конструируем объекты Node и Tree вручную, затем вызываем removeNode(), проверяем (то же).
3. Проход по дереву.
    а) Создаём дерево, создаём итератор, в цикле сдвигаем итератор, проверяя на каждом шаге, что
       мы перешли к определённому элементу.
    б) То же самое, но дерево создаётся вручную.
*/

class Tree;

class Node {
    Node* left, * right, * parent;
    friend class Tree;
    /// находит высоту поддерева, корнем которого является данный элемент
    int find_height()
    {
        int r_height = right ?  right->find_height(): 0;
        int l_height = left ? left->find_height(): 0;
        return r_height > l_height ? r_height + 1 : l_height + 1;
    }

public:
    Node* getLeft() { return left; }
    const Node* getLeft() const { return left; }
    Node* getRight() { return right; }
    const Node* getRight() const { return right; }
    Node* getParent() { return parent; }
    const Node* getParent() const { return parent; }

    std::string name;
    std::string description;
    long price;

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

    ///находит balance_factor
    int find_bf()
    { 
        int r_height = right ? right->find_height() : 0;
        int l_height = left ? left->find_height() : 0;
        return l_height - r_height;
    }
};

class TreeIterator : public std::iterator<std::input_iterator_tag, Node> {
    Node* node;

public:
    TreeIterator() : node(nullptr) {}
    TreeIterator(Node* node_) : node(node_) {}

    bool operator == (const TreeIterator& other) const { return node == other.node; }
    bool operator != (const TreeIterator& other) const { return node != other.node; }

    Node& operator * () { return *node; }
    const Node& operator * () const { return *node; }

    /// TreeIterator it;  ++it      it++

    TreeIterator& operator++() {        /// префиксный   ++it
        /// 1. Если есть элемент справа, берём его.
        /// 2. Иначе, поднимаемся наверх:
        /// 2.1. Если мы уже наверху, то проход закончен (node выставляем в nullptr).
        /// 2.2. Если мы были в левом поддереве, то возвращаем правый элемент текущего родителя
        /// 2.3. Иначе, переходим на шаг 2.

    // TODO
        if (!node->getParent())
        {
            node = nullptr;
            return *this;
        }// Если мы в корне, то мы заканчиваем обход
        if (node->getParent()->getLeft() && node->getParent()->getLeft() == node)
        {
            if (node->getParent()->getRight())
            {
                node = node->getParent()->getRight();
                if (!node->getLeft() && node->getRight())
                    node = node->getRight();
                else if(node->getLeft())
                {
                    while (node->getLeft())
                        node = node->getLeft();
                }
            }
            else
            {
                node = node->getParent();
            }
        }
        else if (node->getParent()->getRight() && node->getParent()->getRight() == node)
        {
            node = node->getParent();
        }
        return *this;
    }
    TreeIterator operator++(int) {      /// постфиксный   it++
        TreeIterator old(node);
        ++* this;
        return old;
    }

    TreeIterator& operator--() {};
    TreeIterator operator--(int) {
        TreeIterator old(node);
        --* this;
        return old;
    }
};

class Tree {
    Node* root;

    /// Вход: b > a, a - родитель b
    /// Выход: b - родитель a
    void smallTurnLeft(Node* a, Node* b) {
        /// 1. Поправить right для родителя (a) ?
        /// 2. Поправить parent (b)
        /// 3. Поправить parent (a)
        /// 4. Переместить левого потомком (b), сделав его правым потомком (a)
        /// 5. Инвертировать взаимосвязь (a) и (b)

        if (a->parent)
            if (a->parent->right && a->parent->right == a)
                a->parent->right = b;
            else
                a->parent->left = b;
        else
            root = b;
        b->parent = a->parent;
        a->parent = b;
        if (b->left) {
            b->left->parent = a;
            a->right = b->left;
        }
        else
            a->right = nullptr;
        b->left = a;
    }

    /// Вход: b > a, и b - родитель a
    /// Выход: a - родитель b
    void smallTurnRight(Node* a, Node* b) {
        if (b->parent)
            if (b->parent->left && b->parent->left == b)
                b->parent->left = a;
            else
                b->parent->right = a;
        else
            root = a;
        a->parent = b->parent;
        b->parent = a;
        if (a->right)
        {
            a->right->parent = b;
            b->left = a->right;
        }
        else
            b->left = nullptr;
        a->right = b;
    }

    /// Вход: a < b, c < b, a - родитель b, b - родитель c
    /// Выход: c - родитель a и b, c->left == a, c->right == b
    void bigTurnLeft(Node* a, Node* b, Node* c) {
        smallTurnRight(c, b);
        smallTurnLeft(a, c);
    }

   /// Вход: a > c > b , a - родитель b, b - родитель c
   ///  Выход: c - родитель a и b, c->left == b, c->right == a
    void bigTurnRight(Node* a, Node* b, Node* c) {
        smallTurnLeft(b, c);
        smallTurnRight(c, a);
    }

    /// балансировка дерева
    void balance(Node* node = nullptr)
    {
        node = node ? node : root;
        int bf = node->find_bf();
        if (bf == -2)
        {
            if (node->right->find_bf() != 1)
                smallTurnLeft(node, node->right);
            else
                bigTurnLeft(node, node->right, node->right->left);
        }
        else if (bf == 2)
        {
            if (node->left->find_bf() != -1)
                smallTurnRight(node->left, node);
            else
                bigTurnRight(node, node->left, node->left->right);
        }
    }

public:
    Node* getRoot() { return root; }
    const Node* getRoot() const { return root; }

    Tree() : root(nullptr) { }
    Node* addNode(const std::string& name) {
        Node* closest = findClosest(name);
        if (closest && closest->name == name)
            return nullptr;
        Node* newNode = new Node(name, closest);
        if (closest && name > closest->name)
            closest->right = newNode;
        else if (closest && name < closest->name)
            closest->left = newNode;
        else
            root = newNode;
        Node* tmp = newNode->parent;
        while (tmp)
        {
            balance(tmp);
            tmp = tmp->parent;
        }
        /// 1. Определиться, будем добавлять левый или правый элемент (родитель - closest).
        /// 2. После добавления поднимаемся на уровень выше (в closest) и проверяем балансировку дерева
        ///    Сбалансировано - если разница высот левого и правого поддеревьев не более 1.
        /// 3. Если не сбалансировано, то выполняем поворот.
        ///    4 вида поворотов:
        ///       малый левый поворот
        ///       малый правый поворот
        ///       большой левый поворот
        ///       большой правый поворот
        ///     Определить вид требуемого поворота и произвести его

        return newNode;
    }

    Node* findNode(const std::string& name) {
        for (auto node = root; node;) {
            auto res = name.compare(node->name);
            if (res == 0)
                return node;
            if (res < 0)
                node = node->left;
            else
                node = node->right;
        }
        return nullptr;
    }

    Node* findMin() {
        Node* node = root;
        if (!node)
            return nullptr;
        while (node->left)
            node = node->left;
        return node;
    }

    Node* findMax() {
        Node* node = root;
        if (!node)
            return nullptr;
        while (node->right)
            node = node->right;
        return node;
    }


    Node* findClosest(const std::string& name)
    {
        Node* node = root;
        while (node)
        {
            if (node->left && name < node->name)
                node = node->left;
            else if (node->right && name > node->name)
                node = node->right;
            else
                return node;
        }
        return node;
    };

    void deleteNode(Node* node)
    {
        if(!node->right && !node->left)
            if (node->parent)
                if(node->parent->left && node->parent->left == node)
                    node->parent->left = nullptr;
                else
                    node->parent->right = nullptr;
            else
                root = nullptr;
        else if(!node->right)
        {
            if (node->parent)
                if (node->parent->left && node->parent->left == node)
                {
                    node->parent->left = node->left;
                    node->left->parent = node->parent;
                }
                else
                {
                    node->parent->right = node->left;
                    node->left->parent = node->parent;
                }
        }
        else
        {
            Node* tmp = node->right->findMin();
            if (node->parent)
                node->parent->right = tmp;
            else
                root = tmp;
            Node* cur = tmp->parent;
            tmp->parent = node->parent;
            tmp->left = node->left;
            tmp->left->parent = tmp;
            if (node->right != tmp)
            {
                tmp->right = node->right;
                tmp->right->parent = tmp;
            }
            while (cur)
            {
                balance(cur);
                cur = cur->parent;
            }
            balance(tmp);
        }
        delete node;
    }

    TreeIterator begin() { return TreeIterator(findMin());}     /// Возвращает итератор, указывающий на минимальный элемент
    TreeIterator end() { return TreeIterator(nullptr); }       /// Возвращает итератор, указывающий на nullptr Node
};


/// TreeIterator it;
/// it++     ++it


#include <map>
#include <set>
#include <vector>


Tree* testTree() {
    Tree* result = new Tree();

    auto elem12 = result->addNode("12");
    assert(elem12->name == "12");
    assert(elem12->description == "");
    assert(elem12->getLeft() == nullptr);
    assert(elem12->getRight() == nullptr);
    assert(elem12->getParent() == nullptr);

    auto elem17 = result->addNode("17");
    assert(elem17->name == "17");
    assert(elem17->description == "");
    assert(elem17->getLeft() == nullptr);
    assert(elem17->getRight() == nullptr);
    assert(elem17->getParent() == elem12);
    assert(elem12->getRight() == elem17);

    auto elem25 = result->addNode("25");//правый малый
    assert(elem25->name == "25");
    assert(elem25->description == "");
    assert(elem25->getLeft() == nullptr);
    assert(elem25->getRight() == nullptr);
    assert(elem25->getParent() == elem17);
    assert(elem12->getParent() == elem17);
    assert(elem17->getLeft() == elem12);
    assert(elem17->getRight() == elem25);

    auto elem5 = result->addNode("05");//левый малый
    assert(elem5->name == "05");
    assert(elem5->description == "");
    assert(elem5->getLeft() == nullptr);
    assert(elem5->getRight() == nullptr);
    assert(elem5->getParent() == elem12);

    auto elem4 = result->addNode("04");
    assert(elem4->name == "04");
    assert(elem4->description == "");
    assert(elem4->getLeft() == nullptr);
    assert(elem4->getRight() == nullptr);
    assert(elem4->getParent() == elem5);
    assert(elem12->getParent() == elem5);
    assert(elem12->getLeft() == nullptr);
    assert(elem12->getRight() == nullptr);
    assert(elem5->getParent() == elem17);
    assert(elem5->getLeft() == elem4);
    assert(elem5->getRight() == elem12);
    assert(elem17->getParent() == nullptr);
    assert(elem17->getLeft() == elem5);
    assert(elem17->getRight() == elem25);

    //проверка большого правого поворота

    auto elem9 = result->addNode("09");
    assert(elem9->name == "09");
    assert(elem9->description == "");
    assert(elem9->getLeft() == nullptr);
    assert(elem9->getRight() == nullptr);
    assert(elem9->getParent() == elem5);
    assert(elem25->getLeft() == nullptr);
    assert(elem25->getRight() == nullptr);
    assert(elem25->getParent() == elem17);
    assert(elem17->getLeft() == nullptr);
    assert(elem17->getRight() == elem25);
    assert(elem17->getParent() == elem12);
    assert(elem12->getLeft() == elem5);
    assert(elem12->getRight() == elem17);
    assert(elem12->getParent() == nullptr);
    assert(elem5->getLeft() == elem4);
    assert(elem5->getRight() == elem9);
    assert(elem5->getParent() == elem12);
    assert(elem4->getLeft() == nullptr);
    assert(elem4->getRight() == nullptr);
    assert(elem4->getParent() == elem5);

    auto elem7 = result->addNode("07");
    assert(elem7->getParent() == elem9);
    assert(elem9->getLeft() == elem7);
    assert(elem7->getLeft() == nullptr);
    assert(elem7->getRight() == nullptr);

    auto elem11 = result->addNode("11");
    assert(elem11->getParent() == elem9);
    assert(elem9->getRight() == elem11);
    assert(elem11->getLeft() == nullptr);
    assert(elem11->getRight() == nullptr);

    auto elem6 = result->addNode("06");
    assert(elem6->getParent() == elem5);
    assert(elem5->getRight() == elem6);
    assert(elem5->getParent() == elem7);
    assert(elem7->getRight() == elem9);
    /// Задача: дописать тест, проверяя малый правый и оба больших поворота
    /// https://gist.github.com/grayed
    
    //тестируем удаление узла
    result->deleteNode(elem11);//удаление листа == можно не проверять.
    result->deleteNode(elem12);
    assert(elem7->getParent() == nullptr);
    assert(elem7->getLeft() == elem5);
    assert(elem7->getRight() == elem17);
    assert(elem5->getParent() == elem7);
    assert(elem5->getLeft() == elem4);
    assert(elem5->getRight() == elem6);
    assert(elem4->getLeft() == nullptr);
    assert(elem4->getRight() == nullptr);
    assert(elem4->getParent() == elem5);
    assert(elem6->getLeft() == nullptr);
    assert(elem6->getRight() == nullptr);
    assert(elem6->getParent() == elem5);
    assert(elem17->getLeft() == elem9);
    assert(elem17->getRight() == elem25);
    assert(elem17->getParent() == elem7);
    assert(elem9->getLeft() == nullptr);
    assert(elem9->getRight() == nullptr);
    assert(elem9->getParent() == elem17);
    assert(elem25->getParent() == elem17);
    assert(elem25->getLeft() == nullptr);
    assert(elem25->getRight() == nullptr);

    //тест итератора
    std::vector<std::string> check;
    check.push_back("04");
    check.push_back("06");
    check.push_back("05");
    check.push_back("09");
    check.push_back("25");
    check.push_back("17");
    check.push_back("07");
    std::vector<std::string>  test;
    for (auto it : *result)
        test.push_back(it.name);
    for (size_t i = 0; i < test.size(); i++)
        assert(test[i] == check[i]);

    return result;
}

int main()
{
    testTree();
}

