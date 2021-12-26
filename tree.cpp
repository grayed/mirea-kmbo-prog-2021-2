#include <iostream>
#include <string>
#include <cassert>

using namespace std;

class TreeIter;
class Tree;

class Node {
private:
    Node *left, *right, *parent;
    friend class Tree;
    friend class TreeIter;

public:
    Node* getLeft() { return left; }
    Node* getRoot() {return parent; }
    Node* getRight() {return right; }
    string desc;
    string name;

    Node() : left(nullptr), right(nullptr), parent(nullptr), name("") {}
    Node(const string& _name, Node* _parent = nullptr) : left(nullptr), right(nullptr), parent(_parent), name(_name) {}


    Node* findMax() {
        Node *node = this;
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    Node* findMin() {
        Node *node = this;
        while (node->left) {
            node = node->left;
        }
        return node;
    }



};

class TreeIter: public iterator<std::input_iterator_tag, Node> {
private:
    Node *node;
public:
    TreeIter() : node(nullptr) {}

    TreeIter(Node* node_) : node(node_) {}

    bool operator==(const TreeIter& other) const {
        return node == other.node;
    }

    bool operator!=(const TreeIter& other) const {
        return !(*this == other);
    }

    const Node* operator->() const { return node; }
    Node* operator->() { return node; }

    const Node& operator*() const { return *node; }
    Node& operator*() { return *node; }

    TreeIter& operator++() {
        if (node->right) {
            node = node->right->findMin();
        } else if (node->parent) {
            while (node->parent->right == node && node->parent) {
                node = node->parent;
            }
            if (node->parent) {
                node = node->parent;
            } else {
                node = nullptr;
            }
        } else {
            node = nullptr;
        }
        return *this;
    }

    TreeIter operator++(int) {
        TreeIter prev(node);
        ++*this;
        return prev;
    }

    TreeIter operator--(int) {
        TreeIter prev(node);
        --*this;
    }

    TreeIter& operator--() {
        if (node->left) {
            node = node->left->findMax();
        } else if (node->parent) {
            while (node->parent->left == node && node->parent) {
                node = node->parent;
            }
            if (node->parent) {
                node = node->parent;
            } else {
                node = nullptr;
            }
        } else {
            node = nullptr;
        }
        return *this;
    }
};


class Tree {
private:
    Node* root;

    void smallTurnLeft(Node* a, Node* b) {
        if (a->parent) {
            if (a->parent->left == a) {
                a->parent->left = b;
            } else {
                a->parent->right = b;
            }
        }
        b->parent = a->parent;
        a->parent = b;
        if (b->left) {
            b->left->parent = a;
        }
        a->right = b->left;
        b->left = a;
    }

    void smallTurnRight(Node* a, Node* b) {
        if (a->parent) {
            if (a->parent->left == a) {
                a->parent->left = b;
            } else {
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


    void bigTurnRight(Node* a, Node* b, Node* c) {
        smallTurnRight(b, c);
        smallTurnLeft(a, c);
    }


    void bigTurnLeft(Node *a, Node *b, Node *c) {
        smallTurnLeft(b, c);
        smallTurnRight(a, c);
    }
public:

    const Node* getRoot() const { return root; }
    Node* getRoot() { return root; }
    Tree() : root(nullptr) {}

    int Height(const Node* node) {
        if (node == nullptr) {
            return 0;
        }
        int left = Height(node->left);
        int right = Height(node->right);
        return max(left, right) + 1;
    }

    Node* getUnbalanced(Node* node) {
        int left, right;
        while (node) {
            left = Height(node->left);
            right = Height(node->right);
            if (abs(left - right) >= 2) {
                return node;
            }
            node = node->parent;
        }
        return nullptr;
    }

    void balancingTree(Node* node) {
        int rHeight = Height(node->right);
        int lHeight = Height(node->left);
        if (lHeight > rHeight) {
            Node* next = node->left;
            rHeight = Height(next->right);
            lHeight = Height(next->left);
            if (lHeight >= rHeight) {
                smallTurnRight(node, next);
            } else {
                bigTurnLeft(node, next, next->right);
            }
        } else {
            Node* next = node->right;
            rHeight = Height(next->right);
            lHeight = Height(next->left);
            if (lHeight <= rHeight) {
                smallTurnLeft(node, next);
            } else {
                bigTurnRight(node, next, next->left);
            }
        }
        if (node == root) {
            root = node->parent;
        }
    }

    Node* findClosest(const string& name) {
        Node* answ = nullptr;
        for (auto node = root; node;) {
            auto res = name.compare(node->name);
            if (res == 0) {
                return node;
            }
            if (res < 0) {
                answ = node;
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        return answ;
    }

    Node* addNode(const std::string& name) {
        Node *closest = findClosest(name);
        if (closest && closest->name == name) {
            return nullptr;
        }
        Node* newNode = new Node(name, closest);
        if (closest == nullptr) {
            Node* maxNode = maxNode->findMax();
            if (maxNode == nullptr) {
                root = newNode;
                return root;
            }
            newNode->parent = maxNode;
            maxNode->right = newNode;
        } else {
            if (closest->left) {
                closest->left->parent = newNode;
            }
            closest->left = newNode;
            newNode->parent = closest;
            newNode->left = closest->left;

        }
        Node* unbalanced = getUnbalanced(newNode->parent);
        if (unbalanced != nullptr) {
            balancingTree(unbalanced);
        }

        return newNode;
    }

    Node* findNode(const string& name) {
        for (auto node = root; node; ) {
            auto res = name.compare(node->name);
                if (res == 0) {
                    return node;
                } else if (res < 0) {
                    node = node->left;
                } else {
                    node = node->right;
                }
        }
        return nullptr;
    }
    void deleteNode(Node* node) {
        Node* closestMin = node->left;

        // ищем макс узел в левом поддереве
        for (auto tmp = node->left; tmp;) {
            closestMin = tmp;
            tmp = tmp->right;
        }

        Node* closest = node->right;
        if (closestMin) {
            closest = closestMin;

            // меняем удаляемый на макс из левого поддерева
            closestMin->right = node->right;
            closestMin->left = nullptr;
            if (closestMin->parent != node) {
                // если есть поддерево
                closestMin->parent->right = closestMin->left;
                if (closestMin->left) {
                    closestMin->left->parent = closestMin->parent;
                }
                closestMin->left = node->left;
                node->left->parent = closestMin;
            }
            closestMin->parent = node->parent;
        }
        if (node->right) {
            if (closestMin) {
                node->right->parent = closestMin;
            }
            else {
                node->right->parent = node->parent;
            }
        }

        Node* parent = node->parent;
        if (parent) {
            // узел в левом или правом поддереве
            auto compRapent = node->name.compare(parent->name);
            if (compRapent < 0) {
                parent->left = closest;
            }
            else {
                parent->right = closest;
            }
        }
        if (node == root) {
            root = closest;
        }
        delete node;
        Node* balanceWhere = parent;
        if (closest) {
            balanceWhere = closest;
        }
        if (balanceWhere) {
            Node* unbalanced = getUnbalanced(balanceWhere);
            if (unbalanced != nullptr) {
                balancingTree(unbalanced);
            }
        }
    }

    Node* findMin() {
        Node* node = root;
        if (!node) {
            return nullptr;
        }
        while (node->left) {
            node = node->left;
        }
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

    TreeIter end() {
        return TreeIter();
    }

    TreeIter begin() {
        return TreeIter(findMin());
    }


};

void testAdd() {
    Tree *result = new Tree();


    // small turn right
    result = new Tree();
    auto a = result->addNode("a");
    assert(a != nullptr);
    assert(a->name == "a");
    assert(a->desc == "");
    assert(a->getRoot() == nullptr);
    assert(a->getLeft() == nullptr);
    assert(a->getRight() == nullptr);

    auto b = result->addNode("b");
    assert(b != nullptr);
    assert(b->name == "b");
    assert(b->desc == "");
    assert(b->getRoot() == a);
    assert(b->getLeft() == nullptr);
    assert(b->getRight() == nullptr);

    auto c = result->addNode("c");
    assert(c != nullptr);
    assert(c->name == "c");
    assert(c->desc == "");
    assert(c->getRoot() == b);
    assert(c->getLeft() == nullptr);
    assert(c->getRight() == nullptr);
    assert(b->getRoot() == nullptr);
    assert(b->getLeft() == a);
    assert(b->getRight() == c);
    assert(a->getRoot() == b);
    assert(a->getLeft() == nullptr);
    assert(a->getRight() == nullptr);

    result = new Tree();
    auto abc = result->addNode("Abc");
    assert(abc != nullptr);
    assert(abc->name == "Abc");
    assert(abc->desc == "");
    assert(abc->getRoot() == nullptr);
    assert(abc->getLeft() == nullptr);
    assert(abc->getRight() == nullptr);

    auto ade = result->addNode("Ade");
    assert(ade != nullptr);
    assert(ade->name == "Ade");
    assert(ade->desc == "");
    assert(ade->getRoot() == abc);
    assert(ade->getRight() == nullptr);
    assert(ade->getLeft() == nullptr);

    auto acd = result->addNode("Acd");
    assert(acd != nullptr);
    assert(acd->name == "Acd");
    assert(acd->desc == "");
    assert(acd->getRoot() == nullptr);
    assert(ade->getRight() == nullptr);
    assert(ade->getRoot() == acd);
    assert(ade->getLeft() == nullptr);
    assert(abc->getLeft() == nullptr);
    assert(abc->getRoot() == acd);
    assert(abc->getRight() == nullptr);

    delete result;

    // Big turn right
    result = new Tree();
    abc = result->addNode("Abc");
    assert(abc != nullptr);
    assert(abc->name == "Abc");
    assert(abc->desc == "");
    assert(abc->getRoot() == nullptr);
    assert(abc->getLeft() == nullptr);
    assert(abc->getRight() == nullptr);

    acd = result->addNode("Acd");
    assert(acd != nullptr);
    assert(acd->name == "Acd");
    assert(acd->desc == "");
    assert(acd->getRoot() == abc);
    assert(acd->getRight() == nullptr);
    assert(acd->getLeft() == nullptr);

    ade = result->addNode("Ade");
    assert(ade != nullptr);
    assert(ade->name == "Ade");
    assert(ade->desc == "");
    assert(ade->getRoot() == acd);
    assert(ade->getRight() == nullptr);
    assert(ade->getLeft() == nullptr);
    assert(abc->getRoot() == acd);
    assert(abc->getLeft() == nullptr);
    assert(abc->getRight() == nullptr);
    assert(acd->getRoot() == nullptr);
    assert(acd->getRight() == ade);
    assert(acd->getLeft() == abc);
}


int main() {
    testAdd();
    
    return 0;
}
