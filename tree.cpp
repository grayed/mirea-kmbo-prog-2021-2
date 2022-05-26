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

class Node
{
    Node* left,
        * right,
        * parent;

    friend class Tree;
    friend class TreeIterator;

public:
    Node* getLeft() { return left; }
    Node* getRight() { return right; }
    Node* getParent() { return parent; }
    const Node* getLeft()   const { return left; }
    const Node* getRight()  const { return right; }
    const Node* getParent() const { return parent; }

    Node* findMin() {
        auto  node = this;
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    Node* findMax() {
        auto  node = this;
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    std::string name;
    std::string description;

    Node(const std::string& name_, Node* parent_ = nullptr) :
        left(nullptr),
        right(nullptr),
        parent(parent_),
        name(name_)
    { }
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
        ++* this;
        return old;
    }

    TreeIterator& operator--() {
        /// Аналогично operator++()
        if (node->getLeft()) {
            node = node->getLeft();  //берем левый элемент
        }
        else {
            Node* temp;
            while (true) {
                temp = node;
                node = node->getParent();
                if (node == nullptr) {
                    return *this;
                }
                else if (temp == node->getRight() && node->getLeft() != nullptr) {
                    node = node->getLeft();
                    return *this;
                }
            }
            return *this;
        }
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
        //else{a->right = nullptr;}
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
        //else{a->left = nullptr;}
        b->right = a;
    }

    void bigTurnLeft(Node* a, Node* b, Node* c) {
        smallTurnRight(b, c);
        smallTurnLeft(a, c);
    }

    void bigTurnRight(Node* a, Node* b, Node* c) {
        smallTurnLeft(b, c);
        smallTurnRight(a, c);
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

        if (closest == nullptr) {
            Node* maxNode = findMax();
            if (maxNode == nullptr) {
                root = newNode;
                return root;
            }
            newNode->parent = maxNode;
            maxNode->right = newNode;
        }
        else {
            if (closest->left) {
                closest->left->parent = newNode;
            }
            newNode->left = closest->left;
            closest->left = newNode;
            newNode->parent = closest;
        }

        Node* test = testToBalance(newNode->parent);
        if (test) {
            int leftHeight = findHeight(test->getLeft());
            int rightHeight = findHeight(test->getRight());
            if (leftHeight > rightHeight) {
                Node* child = test->getLeft();
                leftHeight = findHeight(child->getLeft());
                rightHeight = findHeight(child->getRight());
                if (leftHeight >= rightHeight)
                    smallTurnRight(test, child);
                else
                    bigTurnLeft(test, child, child->getRight());
            }
            else {
                Node* child = test->getRight();
                leftHeight = findHeight(child->getLeft());
                rightHeight = findHeight(child->getRight());
                if (leftHeight <= rightHeight)
                    smallTurnLeft(test, child);
                else
                    bigTurnRight(test, child, child->getLeft());
            }
            if (test == root)
                root = test->parent;
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

    Node* testToBalance(Node* node) {
        int left, right;
        while (node) {
            left = findHeight(node->left);
            right = findHeight(node->right);
            if (abs(left - right) >= 2) {
                return node;
            }
            node = node->parent;
        }
        return nullptr;
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

    int findHeight(const Node* node) {
        if (node == nullptr)
            return 0;
        int left = findHeight(node->getLeft())
            , right = findHeight(node->getRight());
        return std::max(left, right) + 1;
    }

   /// Ищет узел с таким же, или максимально близким справа ("большим") значением name.
    Node* findClosest(const std::string& name) {
        Node* cur = root;
        if (cur) {
            while (true)
            {
                if (cur->right != nullptr && name.compare(cur->right->name) > 0)
                {
                    cur = cur->right;
                }
                else
                {
                    Node* temp = cur;
                    while (temp->left != nullptr && temp->left->right == nullptr && name.compare(temp->getLeft()->name) < 0)
                    {
                        temp = temp->left;
                    }
                    cur = temp;
                }
                break;
            }
        }
        return cur;

    }

    void deleteNode(Node* node) {
    }

    TreeIterator begin() { return findMin(); }     /// Возвращает итератор, указывающий на минимальный элемент
    TreeIterator end() {
        return findMax();
    }       /// Возвращает итератор, указывающий на nullptr Node
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

    auto abb = result->addNode("Abb");
    assert(abb != nullptr);
    assert(abb->name == "Abb");
    assert(abb->description == "");
    assert(abb->getParent() == abc);
    assert(abb->getLeft() == nullptr);
    assert(abb->getRight() == nullptr);
    assert(abc->getParent() == def);
    assert(abc->getLeft() == abb);
    assert(abc->getRight() == nullptr);

    auto ab = result->addNode("Ab");
    assert(ab != nullptr);
    assert(ab->name == "Ab");
    assert(ab->description == "");
    assert(ab->getParent() == abb);
    assert(ab->getLeft() == nullptr);
    assert(ab->getRight() == nullptr);
    assert(abc->getParent() == nullptr);
    assert(abc->getLeft() == abb);
    assert(abc->getRight() == def);
    assert(def->getParent() == abc);
    assert(def->getLeft() == nullptr);
    assert(def->getRight() == ghi);

    auto abbbbb = result->addNode("Abbbbb");
    assert(abbbbb != nullptr);
    assert(abbbbb->name == "Abbbbb");
    assert(abbbbb->description == "");
    assert(abbbbb->getParent() == abb);
    assert(abbbbb->getLeft() == ab);
    assert(abbbbb->getRight() == nullptr);
    assert(abb->getParent() == nullptr);
    assert(abb->getLeft() == abbbbb);
    assert(abb->getRight() == abc);
    assert(abc->getParent() == abb);
    assert(abc->getLeft() == nullptr);
    assert(abc->getRight() == def);

    auto ddf = result->addNode("Ddf");
    assert(ddf != nullptr);
    assert(ddf->name == "Ddf");
    assert(ddf->description == "");
    assert(ddf->getParent() == nullptr);
    assert(ddf->getLeft() == abc);
    assert(ddf->getRight() == def);
    assert(abc->getParent() == ddf);
    assert(abc->getLeft() == abb);
    assert(abc->getRight() == nullptr);
    assert(def->getParent() == ddf);
    assert(def->getLeft() == nullptr);
    assert(def->getRight() == ghi);
    /// Задача: дописать тест, проверяя малый правый и оба больших поворота
    /// https://gist.github.com/grayed
}

int main()
{
    std::cerr << "Test" << std::endl;
    testAddNode();

    return 0;
}
