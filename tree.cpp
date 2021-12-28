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

    /// TreeIterator it;  ++it;  it++

    TreeIterator& operator++() {        /// префиксный   ++it
        /// 1. Если есть элемент справа, берём его.
        /// 2. Иначе, поднимаемся наверх:
        /// 2.1. Если мы уже наверху, то проход закончен (node выставляем в nullptr).
        /// 2.2. Если мы были в левом поддереве, то возвращаем правый элемент текущего родителя
        /// 2.3. Иначе, переходим на шаг 2.
        if (node->getRight())
            node = node->getRight()->findMin();
        else if (node->getParent())
        {
            while (node->getParent() && node->getParent()->getRight() == node)
                node = node->getParent();
            if (node->getParent())
                node = node->getParent();
            else
                node = nullptr;
        }
        else
            node = nullptr;
        return *this;
    }
    TreeIterator operator++(int) {      /// постфиксный   it++
        TreeIterator old(node);
        ++* this;
        return old;
    }

    TreeIterator& operator--() {
        if (node->getLeft())
            node = node->getLeft()->findMax();
        else if (node->getParent())
        {
            while (node->getParent() && node->getParent()->getLeft() == node)
                node = node->getParent();
            if (node->getParent())
                node = node->getParent();
            else
                node = nullptr;
        }
        else
            node = nullptr;
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

    /// Вход: b > a, a - родитель b
    /// Выход: b - родитель a
    void smallTurnLeft(Node* a, Node* b) {
        /// 1. Поправить right для родителя (a)
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
    /// Задача: написать аналогично smallTurnRight, bigTurnLeft, bigTurnRight

    void smallTurnRight(Node* a, Node* b)
    {
        if (a->parent)
            if (a->parent->left && a->parent->left == a)
                a->parent->left = b;
            else
            a->parent->right = b;
        else
            root = b;
        b->parent = a->parent;
        a->parent = b;
        if (b->right) {
            b->right->parent = a;
            a->left = b->right;
        }
        else 
            a->left = nullptr;
        b->right = a;
    }

    void bigTurnLeft(Node* a, Node* b, Node* c)
    {
        smallTurnRight(b, c);
        smallTurnLeft(a, c);
    }

    void bigTurnRight(Node* a, Node* b, Node* c)
    {
        smallTurnLeft(b, c);
        smallTurnRight(a, c);
    }

public:
    Node* getRoot() { return root; }
    const Node* getRoot() const { return root; }

    Tree() : root(nullptr) { }
    Node* addNode(const std::string& name) 
    {
        if (root == nullptr)
        {
            Node* newNode = new Node(name, nullptr);
            root = newNode;
            return root;
        }
        
        Node* closest = findClosest(name);
        if (closest && closest->name == name)
            return nullptr;

        Node* newNode = new Node(name, closest);
        auto name_comparison = name.compare(closest->name);

        if (name_comparison < 0)
        {
            closest->left = newNode;
            newNode->parent = closest;
        }

        else
        {
            closest->right = newNode;
            newNode->parent = closest;
        }

        if (closest == root)
            return newNode;

        auto node = closest->parent;
        if (closest->parent->left == closest)
        {
            if (closest->parent->right == nullptr)
            {
                if (name_comparison < 0)
                    smallTurnRight(closest->parent, closest);
                else
                    bigTurnRight(closest->parent, closest, newNode);
            }
        }

        else
        {
            if (closest->parent->left == nullptr)
            {
                if (name_comparison < 0)
                    bigTurnLeft(closest->parent, closest, newNode);
                else
                    smallTurnLeft(closest->parent, closest);
            }
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

    Node* findMin(Node* n) {
        Node* node = n;
        if (!node)
            return nullptr;
        while (node->left)
            node = node->left;
        return node;
    }

    Node* findMax(Node* n) {
        Node* node = n;
        if (!node)
            return nullptr;
        while (node->right)
            node = node->right;
        return node;
    }

    /// Задача: добавить аналогичные findMin() и findMax() в классе Node;
    /// они должны искать в поддереве, корнем которого является текущий узел.

    /// https://tinyurl.com/899t5ank

    /// Ищет узел с таким же, или максимально близким справа ("большим") значением name.
    Node* findClosest(const std::string& name) {
        Node* closest = root;
        for (auto node = closest; node;)
        {
            auto name_comparison = name.compare(node->name);
            closest = node;
            if (name_comparison < 0)
                node = node->left;
            else if (name_comparison == 0)
                return node;
            else
                node = node->right;
        }
        return closest;
    }

    Node* deleteNode(const std::string& name)
    {
        Node* parent;
        Node* node = root;

        if (root == nullptr)
            return nullptr;
        else
        {
            while (node->name != name)
            {
                if (node->name > name)
                    if (node->left != nullptr)
                        node = node->left;
                    else
                        return nullptr;
                else
                    if (node->right != nullptr)
                        node = node->right;
                    else
                        return nullptr;
            }

            parent = node->parent;

            if (node->left == nullptr && node->right == nullptr)
            {
                if (parent->right == node)
                    parent->right = nullptr;
                else
                    parent->left = nullptr;
                node->parent = nullptr;
            }

            else if (node->left == nullptr)
            {
                if (parent->right = node)
                    parent->right = node->right;
                else
                    parent->left = node->right;

                node->right->parent = parent;
                node->parent = nullptr;
                node->right = nullptr;
            }

            else if (node->right == nullptr)
            {
                if (parent->right = node)
                    parent->right = node->left;
                else
                    parent->left = node->left;

                node->left->parent = parent;
                node->parent = nullptr;
                node->left = nullptr;
            }

            else
            {
                Node* min = findMin(node->right);
                node->name = min->name;
                parent = min->parent;
                if (parent == node)
                    parent->right = nullptr;
                else
                    parent->left = nullptr;
                min->parent = nullptr;
            }
        }
        return node;
    }
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
    // Проверяем повороты

    auto ghi = result->addNode("GHi");
    assert(ghi != nullptr);
    assert(ghi->name == "GHi");
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


    auto aac = result->addNode("AAC");
    assert(aac != nullptr);
    assert(aac->name == "AAC");
    assert(aac->description == "");
    assert(aac->getParent() == abc);
    assert(aac->getLeft() == nullptr);
    assert(aac->getRight() == nullptr);

    auto aab = result->addNode("AAB");
    assert(aab != nullptr);
    assert(aab->name == "AAB");
    assert(aab->description == "");
    assert(aab->getParent() == aac);
    assert(aab->getLeft() == nullptr);
    assert(aab->getRight() == nullptr);
    assert(aac->getParent() == def);
    assert(aac->getLeft() == aab);
    assert(aac->getRight() == abc);


    auto gra = result->addNode("GRa");
    auto gqa = result->addNode("GQA");
    assert(gqa->getParent() == def);
    assert(gqa->getRight() == gra);
    assert(gqa->getLeft() == ghi);
    assert(ghi->getParent() == gqa);
    assert(ghi->getLeft() == nullptr);
    assert(ghi->getRight() == nullptr);
    assert(gra->getParent() == gqa);
    assert(ghi->getLeft() == nullptr);
    assert(ghi->getRight() == nullptr);


    Tree* results = new Tree();
    auto abc1 = results->addNode("Abc");
    assert(abc1 != nullptr);
    assert(abc1->name == "Abc");
    assert(abc1->description == "");
    assert(abc1->getParent() == nullptr);
    assert(abc1->getLeft() == nullptr);
    assert(abc1->getRight() == nullptr);

    auto aac1 = results->addNode("AAC");
    assert(aac1 != nullptr);
    assert(aac1->name == "AAC");
    assert(aac1->description == "");
    assert(aac1->getParent() == abc1);
    assert(aac1->getLeft() == nullptr);
    assert(aac1->getRight() == nullptr);

    auto aad = results->addNode("AAD");
    assert(aad != nullptr);
    assert(aad->name == "AAD");
    assert(aad->description == "");
    assert(aad->getParent() == nullptr);
    assert(aad->getLeft() == aac1);
    assert(aad->getRight() == abc1);
    assert(aac1->getParent() == aad);
    assert(aac1->getLeft() == nullptr);
    assert(aac1->getRight() == nullptr);
    assert(abc1->getParent() == aad);
    assert(abc1->getLeft() == nullptr);
    assert(abc1->getRight() == nullptr);
}

void testIterator()
{
    Tree* tree1 = new Tree();
    auto a = tree1->addNode("31");
    auto b = tree1->addNode("99");
    auto c = tree1->addNode("15");
    TreeIterator it1(c);

    assert(it1 != nullptr);
    assert(it1 == c);
    ++it1;
    assert(it1 == a);
    ++it1;
    assert(it1 == b);
    ++it1;
    assert(it1 == nullptr);

    Tree* tree2 = new Tree();
    auto d = tree2->addNode("5");
    auto e = tree2->addNode("8");
    auto f = tree2->addNode("2");
    TreeIterator it2(e);

    assert(it2 != nullptr);
    assert(it2 == e);
    it2--;
    assert(it2 == d);
    it2--;
    assert(it2 == f);
    it2--;
    assert(it2 == nullptr);
}

void testDeleteNode()
{
    Tree* result = new Tree();
    assert(result->deleteNode("GGT") == nullptr);
    auto abc = result->addNode("Abc");
    auto def = result->addNode("Def");
    auto ghi = result->addNode("GHi");
    auto aac = result->addNode("AAC");
    auto aab = result->addNode("AAB");
    auto gra = result->addNode("GRa");
    auto gqa = result->addNode("GQA");

    assert(result->deleteNode("GHi") == ghi);
    assert(ghi->getParent() == nullptr);
    assert(ghi->getLeft() == nullptr);
    assert(ghi->getRight() == nullptr);
    assert(gqa->getLeft() == nullptr);
    assert(result->deleteNode("GGT") == nullptr);
    assert(result->deleteNode("GQA") == gqa);
    assert(gqa->getParent() == nullptr);
    assert(gqa->getLeft() == nullptr);
    assert(gqa->getRight() == nullptr);
    assert(def->getRight() == gra);
    assert(gra->getRight() == nullptr);
    assert(gra->getLeft() == nullptr);
    assert(gra->getParent() == def);
    assert(result->deleteNode("AAC") == aac);
    assert(aac->getRight() == nullptr);
}


int main()
{
    std::cerr << "Test" << std::endl;
    testAddNode();
    testIterator();
    testDeleteNode();
    return 0;
}
