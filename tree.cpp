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
class TreeIterator;

class Node {
    Node *left, *right, *parent;
    friend class Tree;
    friend class TreeIterator;

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

    Node() : name(""), left(nullptr), right(nullptr), parent(nullptr) {}
    Node(const std::string &name_, Node *parent_ = nullptr)
      : left(nullptr), right(nullptr), parent(parent_), name(name_)
    { }

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

class TreeIterator : public std::iterator<std::input_iterator_tag, Node> 
{
    Node *node;

public:
    TreeIterator() : node(nullptr) {}
    TreeIterator(Node *node_) : node(node_) {}

    bool operator == (const TreeIterator &other) const 
    { 
    	return node == other.node; 
    }
    bool operator != (const TreeIterator &other) const 
    { 
        return node != other.node; 
    }

    Node& operator * () { return *node; }
    const Node& operator * () const { return *node; }

    /// TreeIterator it;  ++it      it++

    TreeIterator& operator++() 
    {        /// префиксный   ++it
        if (node->right) {
            node = node->right->findMin();
        }
        else if (node->parent) {
            while (node->parent && node->parent->right == node) {
                node = node->parent;
            }
            if (node->parent) {
                node = node->parent;
            }
            else {
                node = nullptr; // end, root was reached from right tree
            }
        }
        else {
            node = nullptr;
        }
        return *this;
    }
    
    TreeIterator operator++(int) {      /// постфиксный   it++
        TreeIterator old(node);
        ++*this;
        return old;
    }

    TreeIterator& operator--() {
        if (node->left) {
            node = node->left->findMax();
        }
        else if (node->parent) {
            while (node->parent && node->parent->left == node) {
                node = node->parent;
            }
            if (node->parent) {
                node = node->parent;
            }
            else {
                node = nullptr; // end, root was reached from left tree
            }
        }
        else {
            node = nullptr;
        }

        return *this;
    }
    
    TreeIterator operator--(int) {
        TreeIterator old(node);
        --*this;
        return old;
    }
};

class Tree {
    Node *root;

    void smallTurnLeft(Node *a, Node *b) {
        if (a->parent) 
	{
            if (a->parent->left == a) 
	    {
                a->parent->left = b;
            }
            else 
	    {
                a->parent->right = b;
            }
        }
        b->parent = a->parent;
        a->parent = b;
        if (b->left) 
	{
            b->left->parent = a;
        }
	a->right = b->left;
        b->left = a;
    }

    void smallTurnRight(Node *a, Node *b) 
    {
	if (a->parent) {
            if (a->parent->left == a) {
                a->parent->left = b;
            }
            else {
                a->parent->right = b;
            }
        }
        b->parent = a->parent;
        a->parent = b;
        if (b->right) {
            b->right->parent = a;
        }
        a->left = b->right;
        b->right = a;
    }

    void bigTurnLeft(Node *a, Node *b, Node *c) 
    {
	smallTurnRight(b, c);
        smallTurnLeft(a, c);
    }

    void bigTurnRight(Node *a, Node *b, Node *c) 
    {
	smallTurnLeft(b, c);
        smallTurnRight(a, c);
    }

public:
    Node* getRoot() { return root; }
    const Node* getRoot() const { return root; }

    Tree() : root(nullptr) { }
	
      int getHeight(const Node* node) 
    {
        if (node == nullptr) 
	{
            return 0;
        }
        int left = getHeight(node->left);
        int right = getHeight(node->right);
        return std::max(left, right) + 1;
    }

    Node* findUnbalanced(Node* node) 
    {
        int left, right;
        while (node) 
	    {
            left = getHeight(node->left);
            right = getHeight(node->right);
            if (abs(left - right) >= 2) 
	    {
                return node;  
            }
            node = node->parent;
        }
        return nullptr;
    }
    
    void balancingTree(Node* node) 
    {
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right); 
        Node* newRoot = root;
        if (leftHeight > rightHeight) 
	{
            Node* child = node->left;
            leftHeight = getHeight(child->left);
            rightHeight = getHeight(child->right);
            if (leftHeight >= rightHeight) 
	    {
                smallTurnRight(node, child);
            }
            else 
	    {
                bigTurnLeft(node, child, child->right);
            }
        }
        else 
	{
            Node* child = node->right;
            leftHeight = getHeight(child->left);
            rightHeight = getHeight(child->right);
            if (leftHeight <= rightHeight) 
	    {
                smallTurnLeft(node, child);
            }
            else 
	    {
                bigTurnRight(node, child, child->left);
            }
        }
        if (node == root) 
	{
            root = node->parent;
        }
    }
	
    Node* addNode(const std::string &name) 
    {
        Node *closest = findClosest(name);
        if (closest && closest->name == name)
            return nullptr;
        Node *newNode = new Node(name, closest);

        if (closest == nullptr) {
            Node* maxNode = findMax();
            if (maxNode == nullptr) {
                root = newNode;        
                return root;
            }
            newNode->parent = maxNode;
            maxNode->right = newNode;
        }
        else 
	{
            if (closest->left) 
	    {
                closest->left->parent = newNode;
            }
            newNode->left = closest->left;
            closest->left = newNode;
            newNode->parent = closest;
        }
        Node* unbalanced = findUnbalanced(newNode->parent);
        if (unbalanced != nullptr) 
	{
            balancingTree(unbalanced);
        }
        return newNode;
    }

    Node* findNode(const std::string &name) 
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

    Node* findMin() 
    {
        Node *node = root;
        if (!node)
            return nullptr;
        while (node->left)
            node = node->left;
        return node;
    }

    Node* findMax() 
    {
        Node *node = root;
        if (!node)
            return nullptr;
        while (node->right)
            node = node->right;
        return node;
    }

    /// Ищет узел с таким же, или максимально близким справа ("большим") значением name.
    Node* findClosest(const std::string &name) 
    {
	Node* ans = nullptr;
        for (auto node = root; node;) 
	{
            auto res = name.compare(node->name);
            if (res == 0) 
	    {
                return node;
            }
            if (res < 0) 
	    {
                ans = node;
                node = node->left;
            }
            else 
	    {
                node = node->right;
            }
        }
        return ans;
    }

    void deleteNode(Node* node) 
    {
	Node* closestMin = node->left;

        for (auto temp = node->left; temp;) 
	{
            closestMin = temp; 
            temp = temp->right;
        }
        
        Node* closest = node->right;
        if (closestMin)
	{
            closest = closestMin;

            closestMin->right = node->right;
            closestMin->left = nullptr;
            if (closestMin->parent != node) 
	    {
                // if closestMin has subtree
                closestMin->parent->right = closestMin->left; 
                if (closestMin->left) 
		{
                    closestMin->left->parent = closestMin->parent;
                }
                closestMin->left = node->left;
                node->left->parent = closestMin;
            }
            closestMin->parent = node->parent;
        }
        if (node->right) 
	{
            if (closestMin) 
	    {
                node->right->parent = closestMin;
            }
            else 
	    {
                node->right->parent = node->parent;
            }
        }

        Node* parent = node->parent;
        if (parent) 
	{
            auto compareWithParent = node->name.compare(parent->name);
            if (compareWithParent < 0) 
	    {
                parent->left = closest;
            }
            else 
	    {
                parent->right = closest;
            }
        }
        if (node == root) 
	{
            root = closest;
        }
        delete node;
        Node* balanceFrom = parent;
        if (closest) 
	{
            balanceFrom = closest;
        }
        if (balanceFrom) 
	{
            Node* unbalanced = findUnbalanced(balanceFrom);
            if (unbalanced != nullptr) 
	    {
                balancingTree(unbalanced);
            }
        }
    }

    TreeIterator begin() 
    {
        return TreeIterator(findMin());
    }     
    TreeIterator end() 
    {
        return TreeIterator();
    }       
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

    delete result;
	
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

int main()
{
    std::cerr << "Test" << std::endl;
    testAddNode();

    return 0;
}
