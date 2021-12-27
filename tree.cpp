#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <cassert>

class Tree;

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

class TreeIterator;

class TreeIterator : public std::iterator<std::input_iterator_tag, Node> {
    Node* node;

public:
    TreeIterator() : node(nullptr) {}
    TreeIterator(Node* node_) : node(node_) {}

    bool operator == (const TreeIterator& other) const { return node == other.node; }
    bool operator != (const TreeIterator& other) const { return node != other.node; }

    Node& operator * () { return *node; }
    const Node& operator * () const { return *node; }

    TreeIterator& operator++() {
        auto prev = node;
        if (node->getRight()) {
            node = node->getRight();
            return *this;
        }

        while (node) {
            if (node->getLeft() == prev && node->getRight()) {
                node = node->getRight();
                return *this;
            }
            prev = node;
            node = node->getParent();
        }

        return *this;
    }
    TreeIterator operator++(int) {
        TreeIterator old(node);
        ++* this;
        return old;
    }

    TreeIterator& operator--() {
        auto prev = node;
        if (node->getLeft()) {
            node = node->getLeft();
            return *this;
        }

        while (node) {
            if (node->getRight() == prev && node->getLeft()) {
                node = node->getLeft();
                return *this;
            }
            prev = node;
            node = node->getParent();
        }

        return *this;
    }
    TreeIterator operator--(int) {
        TreeIterator old(node);
        --* this;
        return old;
    }
};


class Tree {
    Node* root;

public:

    /// Вход: b > a, a - родитель b
    /// Выход: b - родитель a
    void smallTurnLeft(Node* a, Node* b) {
        if (a->parent)
            a->parent->right = b;
        b->parent = a->parent;
        a->parent = b;
        if (b->left) {
            b->left->parent = a;
            a->right = b->left;
        }
        b->left = a;
    }

    void smallTurnRight(Node* a, Node* b) {
        if (a->parent)
            a->parent->left = b;
        b->parent = a->parent;
        a->parent = b;
        if (b->right) {
            b->right->parent = a;
            a->left = b->right;
        }
        b->right = a;
    }


    void bigTurnLeft(Node* a, Node* b, Node* c) {
        smallTurnLeft(b, c);
        smallTurnRight(a, c);
    }

    void bigTurnRight(Node* a, Node* b, Node* c) {
        smallTurnRight(b, c);
        smallTurnLeft(a, c);
    }

    int getHeight(Node* a) {
        if (a == nullptr) return 0;

        return std::max(getHeight(a->left), getHeight(a->right));
    }

    void balance(Node* a, Node* b, Node* c) {
        if (getHeight(b) - getHeight(a->left) == 2
            && getHeight(b->left) <= getHeight(b->right))
            smallTurnLeft(a, b);

        if (getHeight(b) - getHeight(a->left) == 2
            && getHeight(c) > getHeight(b->right))
            bigTurnLeft(a, b, c);

        if (getHeight(b) - getHeight(a->right) == 2
            && getHeight(b->right) <= getHeight(b->left))
            smallTurnRight(a, b);

        if (getHeight(b) - getHeight(a->right) == 2
            && getHeight(c) > getHeight(b->left))
            bigTurnRight(a, b, c);
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

        if (!root) return newNode;

        newNode->left = closest->left;
        closest->left = newNode;

        balance(closest, closest->left, closest->left->left);

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

    /// Ищет узел с таким же, или максимально близким справа ("большим") значением name.
    Node* findClosest(const std::string& name) {
        auto ans = root;
        for (auto node = root; node;) {
            auto res = name.compare(node->name);
            if (res < 0) {
                ans = node;
                if (node->left)
                    node = node->left;
                else return ans;
            }
            else {
                if (node->right)
                    node = node->right;
                else return nullptr;
            }
        }
        return ans;
    }

    void deleteNode(Node* node) {
    }

    TreeIterator begin() { return TreeIterator(this->findMin()); }     /// Возвращает итератор, указывающий на минимальный элемент
    TreeIterator end() { return TreeIterator(); }       /// Возвращает итератор, указывающий на nullptr Node
};


/// TreeIterator it;
/// it++     ++it


#include <map>
#include <set>

void testAddNode() {
    Tree* result = new Tree();

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

int main()
{
    Tree tree;
    tree.addNode("First");
    tree.addNode("Second");
    tree.addNode("Third");
    std::cout << "Our tree:" << std::endl;
    for (auto& node : tree) {
        std::cout << node.name << ": " << node.description << std::endl;
    }

    std::set<std::string> setOfStrings;
    std::map<std::string, std::string> myMap;

    setOfStrings.insert("First");
    setOfStrings.insert("Second");
    setOfStrings.insert("Third");
    setOfStrings.insert("First");
    setOfStrings.insert("First");
    std::cout << "Set contains:" << std::endl;
    for (auto s : setOfStrings)
        std::cout << s << std::endl;
    std::cout << std::endl;

    myMap["First"] = "La-la-la";
    myMap["Second"] = "La-la-la";
    myMap["Third"] = "La-la-la";
    myMap["First"] = "Fa-fa-fa";
    std::cout << "Map contains:" << std::endl;
    for (auto s : myMap)    /// std::pair<std::string, std::string>
        std::cout << s.first << " -> " << s.second << std::endl;
    std::cout << std::endl;


    return 0;
}
