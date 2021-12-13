#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cassert>
using namespace std;
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

    string name;
    string description;
    long price;

   Node(const string& name, Node* parent = nullptr)
   	: left(nullptr), right(nullptr), parent(parent), name(name) {}

    Node* findMin() {
        Node *node = this;
        while (node->left)
            node = node->left;
        return node;
    }

    Node* findMax() {
        Node *node = this;
        while (node->right)
            node = node->right;
        return node;
    }
};

class TreeIterator;
class Tree {
public:
    Node* root;
    Tree() : root(nullptr) {}
    Node* findNode(const string& name)
    {
        for (auto node = root; node;)
        {
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
    void smallTurnLeft(Node* a, Node* b)
    {
        if (a->parent)
            a->parent->right = b;
        b->parent = a->parent;
        a->parent = b;
        if (b->left) {
            b->left->parent = a;
            a->right = b->left;
        }
        b->left = a;
	if (a == root)
            root = b;
    }

    void smallTurnRight(Node *a, Node *b) {
	if (a->parent)
            a->parent->left = b;
        b->parent = a->parent;
        a->parent = b;
        if (b->right)
        {
            b->right->parent = a;
            a->left = b->right;
        }
        b->right = a;
        if (a == root)
            root = b;
    }

    void bigTurnLeft(Node *a, Node *b, Node *c) {
	smallTurnRight(b, c);
        b->left = nullptr;
        smallTurnLeft(a, c);
        a->right = nullptr;
        a->left = nullptr;
    }

    void bigTurnRight(Node *a, Node *b, Node *c) {
	smallTurnLeft(b, c);
        b->right = nullptr;
        smallTurnRight(a, c);
        a->right = nullptr;
        a->left = nullptr;
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

    /// Ищет узел с таким же, или максимально близким справа ("большим") значением name.
    Node* findClosest(const std::string &name) {
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
    assert(abc->getRight() == def);

    /// Проверяем малый левый поворот
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

    /// Задача: дописать тест, проверяя малый правый и оба больших поворота
    /// https://gist.github.com/grayed
}

int main()
{
    std::cerr << "Test" << std::endl;
    testAddNode();

    return 0;
}

