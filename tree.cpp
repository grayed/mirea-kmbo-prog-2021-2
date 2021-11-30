#include <cassert>
#include <iostream>
#include <string>

///
/// Домашнее задание:
///
/// 1.  Добавить findMin() и findMax() в классе Node, аналогичные таковым в Tree;
///     они должны искать в поддереве, корнем которого является текущий узел.
///
/// 2.  Реализовать малый правый поворот, а также большие левый и правый повороты
///     в классе Tree, по аналогии с малым левым поворотом.
///
/// 3.  Реализовать префиксный оператор «++» для итератора.
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
    Node *left, *right, *parent;
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
    long price;

    Node(const std::string &name_, Node *parent_ = nullptr)
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

class TreeIterator : public std::iterator<std::input_iterator_tag, Node> {
    Node *node;

public:
    TreeIterator() : node(nullptr) {}
    TreeIterator(Node *node_) : node(node_) {}

    bool operator == (const TreeIterator &other) const { return node == other.node; }
    bool operator != (const TreeIterator &other) const { return node != other.node; }

    Node& operator * () { return *node; }
    const Node& operator * () const { return *node; }

    /// TreeIterator it;  ++it      it++

    TreeIterator& operator++() {        /// префиксный   ++it
        TreeIterator old(node);
        if (node->getRight() != nullptr)
        {
            node = node->getRight();
        }
        else
        {
            Node* temp;
            while (true)
            {
                temp = node;
                node = node->getParent();
                if (node == nullptr)
                    return *this;
                else if (temp == node->getLeft() && node->getRight() != nullptr)
                {
                    node = node->getRight();
                    return *this;
                }
            }
        }
        return *this;
    }
	
    TreeIterator operator++(int) {      /// постфиксный   it++
        TreeIterator old(node);
        ++*this;
        return old;
    }

    TreeIterator& operator--() {
        /// Аналогично operator++()
    }
    TreeIterator operator--(int) {
        TreeIterator old(node);
        --*this;
        return old;
    }
};

class Tree {
    Node *root;

    void smallTurnLeft(Node* a, Node* b) {

        if (a->parent)
            a->parent->right = b;
        b->parent = a->parent;
        a->parent = b;
        if (b->left)
        {
            b->left->parent = a;
            a->right = b->left;
        }
        else
        {
            a->right = nullptr;
        }
        b->left = a;
    }

    void smallTurnRight(Node* a, Node* b) {
        if (a->parent)
            a->parent->left = b;
        b->parent = a->parent;
        a->parent = b;
        if (b->right)
        {
            b->right->parent = a;
            a->left = b->right;
        }
        else
        {
            a->left = nullptr;
        }
        b->right = a;
    }

    void bigTurnRight(Node* a, Node* b, Node* c) {
        smallTurnRight(b, c);
        smallTurnLeft(a, c);
    }

    void bigTurnLeft(Node* a, Node* b, Node* c) {
        smallTurnLeft(b, c);
        smallTurnRight(a, c);
    }

public:
    Node* getRoot() { return root; }
    const Node* getRoot() const { return root; }

    Tree() : root(nullptr) { }
    Node* addNode(const std::string &name) {
        Node *closest = findClosest(name);
        if (closest && closest->name == name)
            return nullptr;
        Node *newNode = new Node(name, closest);

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

    Node* findNode(const std::string &name) {
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
        Node *node = root;
        if (!node)
            return nullptr;
        while (node->left)
            node = node->left;
        return node;
    }

    Node* findMax() {
        Node *node = root;
        if (!node)
            return nullptr;
        while (node->right)
            node = node->right;
        return node;
    }

    Node* findClosest(const string& name)
    {
        Node* cur = root;
        while (true)
        {
            if (cur->right != nullptr && name.compare(cur->right->name) > 0)
            {
                cur = cur->right;
            }
            else
            {
                Node* temp = cur;
                while (temp->left != nullptr && temp->left->right == nullptr && name.compare(temp->left->name) < 0)
                {
                    temp = temp->left;
                }
                cur = temp;
            }
            break;
        }
        return cur;
    }

    void deleteNode(Node* node) {
    }

    TreeIterator begin() {}     /// Возвращает итератор, указывающий на минимальный элемент
    TreeIterator end() {}       /// Возвращает итератор, указывающий на nullptr Node
};


/// TreeIterator it;
/// it++     ++it


#include <map>
#include <set>


void testAddNode() {
    Tree *result = new Tree();

    auto abc = result->addNode("Abc");
    assert(abc != nullptr);
    assert(abc->name == "Abc");
    assert(abc->description == "");
    assert(abc->getParent() == nullptr);
    assert(abc->getLeft() == nullptr);
    assert(abc->getRight() == nullptr);

    auto def = result->addNode("Def");
    assert(def != nullptr);
    assert(def->name == "Def");
    assert(def->description == "");
    assert(def->getParent() == abc);
    assert(def->getLeft() == nullptr);
    assert(def->getRight() == nullptr);
    assert(abc->getRight() == def);

    /// повороты
    auto ghi = result->addNode("Ghi");
    assert(ghi != nullptr);
    assert(ghi->name == "Ghi");
    assert(ghi->description == "");
    assert(ghi->getParent() == def);
    assert(ghi->getLeft() == nullptr);
    assert(ghi->getRight() == nullptr);
    assert(def->getParent() == nullptr);
    assert(def->getLeft() == abc);
    assert(def->getRight() == ghi);
    assert(abc->getParent() == def);
    assert(abc->getLeft() == nullptr);
    assert(abc->getRight() == nullptr);
    assert(result->getRoot() == def);
    assert(result->getRoot()->getRight() == ghi);
    assert(result->getRoot()->getLeft() == abc);

    auto abb = result->addNode("Abb");
    auto aba = result->addNode("Aba");
    assert(aba != nullptr);
    assert(aba->name == "Aba");
    assert(aba->description == "");
    assert(aba->getParent() == abb);
    assert(aba->getLeft() == nullptr);
    assert(aba->getRight() == nullptr);
    assert(abb->getParent() == abc);
    assert(abc->getParent() == nullptr);
    assert(abb->getLeft() == aba);
    assert(abb->getRight() == nullptr);
    assert(abc->getParent() == nullptr);
    assert(abc->getLeft() == abb);
    assert(abc->getRight() == def);
    assert(def->getParent() == abc);
    assert(def->getLeft() == nullptr);
    assert(def->getRight() == ghi);
    assert(ghi->getParent() == def);
    assert(ghi->getLeft() == nullptr);
    assert(ghi->getRight() == nullptr);
    
    //

    //addNode

    auto eee = result->addNode("Eee");
    assert(def->getRight() == eee);
    assert(eee->getParent() == def);
    assert(eee->getLeft() == nullptr);
    assert(eee->getRight() == ghi);

}

int main()
{
    std::cerr << "Test" << std::endl;
    testAddNode();

    return 0;
}

