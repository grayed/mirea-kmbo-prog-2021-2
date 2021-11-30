#include <iostream>
#include <vector>
#include <algorithm>

class Node
{
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
    {}

    Node* findMin()
    {
        Node *node = this;
        while (node -> left)
            node = node -> left;
        return node;
    }

    Node* findMax()
    {
        Node *node = this;
        while (node -> right)
            node = node -> right;
        return node;
    }
};

class TreeIterator : public std::iterator<std::input_iterator_tag, Node>
{
    Node *node;

public:
    TreeIterator() : node(nullptr) {}
    TreeIterator(Node *node_) : node(node_) {}

    bool operator == (const TreeIterator &other) const { return node == other.node; }
    bool operator != (const TreeIterator &other) const { return node != other.node; }

    Node& operator * () { return *node; }
    const Node& operator * () const { return *node; }

    TreeIterator& operator++()
    {
        if (node -> getRight())
        {
            node = node -> getRight();
            while (node -> getLeft()) node = node -> getLeft();
            return *this;
        }

        Node *prev_node = node;
        node = node -> getParent();
        while(node)
        {
            if (node -> getRight() != prev_node)
            {
                node = node -> getRight();
                while (node -> getLeft()) node = node -> getLeft();
                return *this;
            }
            prev_node = node;
            node = node -> getParent();
        }
        return *this;
    }
    TreeIterator operator++(int)
    {
        TreeIterator old(node);
        ++*this;
        return old;
    }

    TreeIterator operator--()
    {
        if (node -> getLeft())
        {
            node = node -> getLeft();
            while (node -> getRight()) node = node -> getRight();
            return *this;
        }

        Node *prev_node = node;
        node = node -> getParent();
        while(node)
        {
            if (node -> getLeft() != prev_node)
            {
                node = node -> getLeft();
                while (node -> getRight()) node = node -> getRight();
                return *this;
            }
            prev_node = node;
            node = node -> getParent();
        }
        return *this;
    }
    TreeIterator operator--(int)
    {
        TreeIterator old(node);
        --*this;
        return old;
    }
};

class Tree
{
    Node *root;

public:
    Tree() : root(nullptr) { }
    Node* addNode(const std::string &name){}
    Node* findNode(const std::string &name)
    {
        for (auto node = root; node;)
        {
            auto res = name.compare(node -> name);
            if (res == 0)
                return node;
            if (res < 0)
                node = node -> left;
            else
                node = node -> right;
        }
        return nullptr;
    }

    Node* findClosest(const std::string &name)
    {
        Node* closest = nullptr;
        for (auto node = root; node;)
        {
            auto res = name.compare(node -> name);
            if (res == 0)
                return node;
            if (res > 0)
                node = node -> right;
            else
            {
                closest = node;
                node = node -> left;
            }
        }
        return closest;        
    }

    Node* findMin()
    {
        Node *node = root;
        if (!node)
            return nullptr;
        while (node -> left)
            node = node -> left;
        return node;
    }

    Node* findMax()
    {
        Node *node = root;
        if (!node)
            return nullptr;
        while (node -> right)
            node = node -> right;
        return node;
    }

    void deleteNode(Node* node)
    {
        // Node* closestMin = node -> left;

        // // find max node in left subtree of node being deleted
        // for (auto temp = node -> left; temp;)
        // {
        //     closestMin = temp;
        //     temp = temp -> right;
        // }

        // // if closestMin has subtree
        // closestMin -> parent -> right = closestMin -> left;

        // // changing node to be deleted to max node in left subtree
        // closestMin -> parent = node -> parent;
        // closestMin -> left = node -> left;
        // closestMin -> right = node -> right;

        // // find where is node (left of right subtree)
        // auto compareWithParent = node -> name.compare(node -> parent -> name);
        // if (compareWithParent < 0)
        // {
        //     node -> parent -> left = closestMin;
        // }
        // else
        // {
        //     node -> parent -> right = closestMin;
        // }
        // delete node;
    }

    TreeIterator begin() {}
    TreeIterator end() {}
};

