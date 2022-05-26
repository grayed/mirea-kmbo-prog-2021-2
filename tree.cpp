#include <iostream>
#include <string>
#include <cassert>

class Tree;
class TreeIterator;

class Node {
private:
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

    Node() : name(""), left(nullptr), right(nullptr), parent(nullptr) {}
    Node(const std::string& _name, Node* _parent = nullptr)  
        : name(_name), left(nullptr), right(nullptr), parent(_parent) {}

    Node* findMin() {
        Node* node = this;
        while (node->left) {
            node = node->left;
        }
        return node;
    }
    
    Node* findMax() {
        Node* node = this;
        while (node->right) {
            node = node->right;
        }
        return node;
    }
};

class TreeIterator: public std::iterator<std::input_iterator_tag, Node> {
private:
    Node* node;
public:
    TreeIterator() : node(nullptr) {}
    TreeIterator(Node* node_) : node(node_) {}
    
    bool operator==(const TreeIterator& other) const {
        return node == other.node;
    }
    
    bool operator!=(const TreeIterator& other) const {
        return !(*this == other);
    }

    Node& operator*() { return *node; }
    const Node& operator*() const { return *node; }

    Node* operator->() { return node; }
    const Node* operator->() const { return node; }
    
    TreeIterator& operator++() {
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
    
    TreeIterator operator++(int) {
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
private:
    Node* root;

    void smallTurnLeft(Node* a, Node* b) {
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

    Tree() : root(nullptr) {}

    int getHeight(const Node* node) {
        if (node == nullptr) {
            return 0;
        }
        int left = getHeight(node->left);
        int right = getHeight(node->right);
        return std::max(left, right) + 1;
    }

    Node* findUnbalanced(Node* node) {
        int left, right;
        while (node) {
            left = getHeight(node->left);
            right = getHeight(node->right);
            if (abs(left - right) >= 2) {
                return node;  
            }
            node = node->parent;
        }
        return nullptr;
    }
    
    void balancingTree(Node* node) {
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right); 
        Node* newRoot = root;
        if (leftHeight > rightHeight) {
            Node* child = node->left;
            leftHeight = getHeight(child->left);
            rightHeight = getHeight(child->right);
            if (leftHeight >= rightHeight) {
                smallTurnRight(node, child);
            }
            else {
                bigTurnLeft(node, child, child->right);
            }
        }
        else {
            Node* child = node->right;
            leftHeight = getHeight(child->left);
            rightHeight = getHeight(child->right);
            if (leftHeight <= rightHeight) {
                smallTurnLeft(node, child);
            }
            else {
                bigTurnRight(node, child, child->left);
            }
        }
        if (node == root) {
            root = node->parent;
        }
    }

    Node* addNode(const std::string& name) {
        Node* closest = findClosest(name);
        if (closest && closest->name == name) {
            return nullptr;
        }
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
        Node* unbalanced = findUnbalanced(newNode->parent);
        if (unbalanced != nullptr) {
            balancingTree(unbalanced);
        }

        return newNode;
    }

    Node* findNode(const std::string& name) {
        for (auto node = root; node;) {
            auto res = name.compare(node->name);
            if (res == 0) {
                return node;
            }
            else if (res < 0) {
                node = node->left;
            }
            else {
                node = node->right;
            }
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

    void deleteNode(Node* node) {}

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
 
int main() {
    testAddNode();

    return 0;
}
