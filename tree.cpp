#include <cassert>
#include <iostream>
#include <string>

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

    TreeIterator& operator++() {        
        
        return *this;
    }
    TreeIterator operator++(int) {      /// постфиксный   it++
        TreeIterator old(node);
        ++* this;
        return old;
    }

    TreeIterator& operator--() {
        /// Аналогично operator++()
    }
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
        /// 1. Поправить right для родителя (a)
        /// 2. Поправить parent (b)
        /// 3. Поправить parent (a)
        /// 4. Переместить левого потомком (b), сделав его правым потомком (a)
        /// 5. Инвертировать взаимосвязь (a) и (b)

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

    /// Задача: написать аналогично smallTurnRight, bigTurnLeft, bigTurnRight

    void smallTurnRight(Node* a, Node* b) {
        if (a->parent)
            a->parent->left = b;
        b->parent = a->parent;
        a->parent = b;
        if (b->left) {
            b->left->parent = a;
            a->left = b->right;
        }
        b->right = a;
    }

    void bigTurnLeft(Node* a, Node* b, Node* c)
    {
        smallTurnRight(b, c);
        b->left = nullptr;
        smallTurnLeft(a, c);
        a->right = nullptr;
        a->left = nullptr;
    }

    void bigTurnRight(Node* a, Node* b, Node* c)
    {
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
    Node* addNode(const std::string& name) {
        Node* closest = findClosest(name);
        if (closest && closest->name == name)
            return nullptr;
        Node* newNode = new Node(name, closest);

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
	
    Node* findClosest(const std::string& name) {
        Node* ans = nullptr;
        for (auto node = root; node;) {
            auto res = name.compare(node->name);
            if (res == 0) {
                return node;
            }
            if (res < 0) {
                ans = node;
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        return ans;
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
	
 Node* findMax() {
        Node* node = root;
        if (!node) {
            return nullptr;
        }
        while (node->right) {
            node = node->right;
        }
        return node;
    }
    
    TreeIterator begin() {
        return TreeIterator(findMin());
    }
    TreeIterator end() {
        return TreeIterator();
    }
};

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

    // small turn left 
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

    delete result;

    // small turn right 
    result = new Tree();
    auto a = result->addNode("a");
    assert(a != nullptr);
    assert(a->name == "a");
    assert(a->description == "");
    assert(a->getParent() == nullptr);
    assert(a->getLeft() == nullptr);
    assert(a->getRight() == nullptr);

    auto b = result->addNode("b");
    assert(b != nullptr);
    assert(b->name == "b");
    assert(b->description == "");
    assert(b->getParent() == a);
    assert(b->getLeft() == nullptr);
    assert(b->getRight() == nullptr);

    auto c = result->addNode("c");
    assert(c != nullptr);
    assert(c->name == "c");
    assert(c->description == "");
    assert(c->getParent() == b);
    assert(c->getLeft() == nullptr);
    assert(c->getRight() == nullptr);
    assert(b->getParent() == nullptr); 
    assert(b->getLeft() == a);
    assert(b->getRight() == c);
    assert(a->getParent() == b);
    assert(a->getLeft() == nullptr);
    assert(a->getRight() == nullptr);

    // big turn left 
    result = new Tree();
    abc = result->addNode("Abc");
    assert(abc != nullptr);
    assert(abc->name == "Abc");
    assert(abc->description == "");
    assert(abc->getParent() == nullptr);
    assert(abc->getLeft() == nullptr);
    assert(abc->getRight() == nullptr);

    auto ade = result->addNode("Ade");
    assert(ade != nullptr);
    assert(ade->name == "Ade");
    assert(ade->description == "");
    assert(ade->getParent() == abc);
    assert(ade->getRight() == nullptr);
    assert(ade->getLeft() == nullptr);

    auto acd = result->addNode("Acd");
    assert(acd != nullptr);
    assert(acd->name == "Acd");
    assert(acd->description == "");
    assert(acd->getParent() == nullptr);
    assert(ade->getRight() == nullptr);
    assert(ade->getParent() == acd);
    assert(ade->getLeft() == nullptr);
    assert(abc->getLeft() == nullptr);
    assert(abc->getParent() == acd);
    assert(abc->getRight() == nullptr);

    delete result;

    // Big turn right 
    result = new Tree();
    abc = result->addNode("Abc");
    assert(abc != nullptr);
    assert(abc->name == "Abc");
    assert(abc->description == "");
    assert(abc->getParent() == nullptr);
    assert(abc->getLeft() == nullptr);
    assert(abc->getRight() == nullptr);

    acd = result->addNode("Acd");
    assert(acd != nullptr);
    assert(acd->name == "Acd");
    assert(acd->description == "");
    assert(acd->getParent() == abc);
    assert(acd->getRight() == nullptr);
    assert(acd->getLeft() == nullptr);

    ade = result->addNode("Ade");
    assert(ade != nullptr);
    assert(ade->name == "Ade");
    assert(ade->description == "");
    assert(ade->getParent() == acd);
    assert(ade->getRight() == nullptr);
    assert(ade->getLeft() == nullptr);
    assert(abc->getParent() == acd);
    assert(abc->getLeft() == nullptr);
    assert(abc->getRight() == nullptr);
    assert(acd->getParent() == nullptr);
    assert(acd->getRight() == ade);
    assert(acd->getLeft() == abc);
}

void testDeleteNode() {
    Tree* result = new Tree();
    for (int i = 0; i < 26; i++) {
        result->addNode(std::string(1, 'a' + i));
    }
    Node* deletedNode = result->findNode("z");
    assert(deletedNode != nullptr);
    result->deleteNode(deletedNode);
    for (auto it = result->begin(); it != result->end(); it++) {
        assert(it->name != "z");
        if (it->getLeft() != nullptr) {
            assert(it->name > it->getLeft()->name);
        }
        if (it->getRight() != nullptr) {
            assert(it->name < it->getRight()->name);
        }
    }
    for (int i = 0; i < 25; i++) {
        std::string s(1, 'a' + i);
        Node* deletedNode = result->findNode(s);
        assert(deletedNode != nullptr);
        result->deleteNode(deletedNode);
        for (auto it = result->begin(); it != result->end(); it++) {
            assert(it->name != s);
            if (it->getLeft() != nullptr) {
                assert(it->name > it->getLeft()->name);
            }
            if (it->getRight() != nullptr) {
                assert(it->name < it->getRight()->name);
            }
        }
    }
    assert(result->begin() == nullptr);
    result->addNode("z");
    assert(result->begin()->name == "z");
}

void testIterator() {
    Tree* result = new Tree();
    for (int i = 0; i < 26; i++) {
        result->addNode(std::string(1, 'a' + i));
    }
    char tempElement = 'a';
    for (auto it = result->begin(); it != result->end(); it++) {
        assert(std::string(1, tempElement) == it->name);
        tempElement++;
    }
}
 
int main() {
    testAddNode();
    testDeleteNode();
    testIterator();

    return 0;
}
