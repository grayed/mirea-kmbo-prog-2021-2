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
class Tree
{
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

    void smallTurnLeft(Node* a, Node* b){
        if (a->parent)
        a->parent->right = b;
        b->parent = a->parent;
        a->parent = b;
        if (b->left) 
	{
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

 Node* findClosest(const string& name)
    {
        Node* privious = root;;
        for (auto node = root; node;)
        {
		auto res = name.compare(node->name);
            	if (res == 0)
                	return node;
            	if (res < 0)
	 	{
                	privious = node;
                	node = node->left;
            	}
            	else
           	{
                	privious = node;
                	node = node->right;
            	}
        	}
		return privious;
    	}
	Node* addNode(const string& name)
   	{

        	if (root == nullptr)
        	{
            		Node* newNode = new Node(name, nullptr);
            		root = newNode;
            		return newNode;
        	}
        	Node* closest = findClosest(name);
        	if (closest && closest->name == name)
            	    return nullptr;
        	Node* newNode = new Node(name, closest);
        	auto res = name.compare(closest->name);
        	if (res < 0)
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
        	auto d1 = closest->parent;
        	if (closest->parent->left == closest)
        	{
            		if (closest->parent->right == nullptr)
            		{
                		if (res < 0)
                		{
                    			smallTurnRight(closest->parent, closest);
                    			d1->left = nullptr;
                		}
               			else
                		{
                    			bigTurnRight(closest->parent, closest, newNode);
                		}
            		}

		}
        	else
        	{
            	if (closest->parent->left == nullptr)
            	{

                	if (res < 0)
                	{
                    		bigTurnLeft( closest->parent, closest, newNode);

                	}
                	else
                	{
                    		smallTurnLeft(closest->parent, closest);
                    		d1->right = nullptr;

                	}


            	}
        	}
        return newNode;

    	}
    Node* findMinT(Node* a)
    {
        Node* node = a;
        if (!node)
            return nullptr;
        while (node->left)
            node = node->left;
        return node;
    }

    Node* findMaxT(Node* a)
    {
        Node* node = a;
        if (!node)
            return nullptr;
        while (node->right)
            node = node->right;
        return node;
    }

    Node* findMaxT(Node* a)
    {
        Node* node = a;
        if (!node)
            return nullptr;
        while (node->right)
            node = node->right;
        return node;
    }
	 Node* findMax()
    {
        Node* node = root;
        if (!node)
            return nullptr;
        while (node->right)
            node = node->right;
        return node;
    }
	 Node* delNode(const string &name)
    {

         Node* privious;
         Node* node = root;

         if (root == NULL)
         {
             return nullptr;
         }
         else
         {
             while (node->name != name)
             {
                 if (name < node->name)

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
             if ((node->left == NULL) && (node->right == NULL))
             {
                if (node->parent->right == node)
                	node->parent->right = nullptr;
                else
                	node->parent->left = nullptr;
                	node->parent = nullptr;
             }
             else if (node->left == NULL)
             {

                 if (node->parent->right == node)
                 {
                     privious = node->parent;
                     node->parent->right = node->right;
                     node->right->parent = privious;
                     node->parent = nullptr;
                     node->right = nullptr;

                 }
                 else
                 {
                     privious = node->parent;
                     node->parent->left = node->right;
                     node->right->parent = privious;
                     node->parent = nullptr;
                     node->right = nullptr;
                 }
             }
             else if (node->right == NULL)
             {

                if (node->parent->right == node)
                {
                    privious = node->parent;
                    node->parent->right = node->left;
                    node->left->parent = privious;
                    node->parent = nullptr;
                    node->left = nullptr;

                }
                else
                {
                    privious = node->parent;
                    node->parent->left = node->left;
                    node->left->parent = privious;
                    node->left = nullptr;
                }

             }
             else
             {

                Node* temp = findMinT(node->right);
                node->name = temp->name;
                privious = temp->parent;
                if (privious == node)
                {
                    privious->right = NULL;
                }
                else
                {
                    privious->left = NULL;
                }
                temp->parent = NULL;
             }
         }

         return node;
    }
	};
class TreeIterator : public std::iterator<std::input_iterator_tag, Node>
{
    Node* node;

public:
    TreeIterator() : node(nullptr) {}
    TreeIterator(Node* node_) : node(node_) {}

    bool operator == (const TreeIterator& other) const { return node == other.node; }
    bool operator != (const TreeIterator& other) const { return node != other.node; }
    const Node& operator * () const { return *node; }

    TreeIterator& operator++() //Префексный
    {

       if (node->getRight()) {
            node = node->getRight()->findMinT();
        }
        else if (node->getParent()) 
        {
            while (node->getParent() && node->getParent()->getRight() == node)
            {
                node = node->getParent();
            }
            if (node->getParent())
            {
                node = node->getParent();
            }
            else 
	    {
                node = nullptr; 
            }
        }
        else 
	{
            node = nullptr;
        }
        return *this;
    }
    TreeIterator operator++(int) //Постфиксный
    {
        TreeIterator old(node);
        ++* this;
        return old;
    }

    TreeIterator& operator--() 
    {
 	if (node->getLeft())
        {
            node = node->getLeft()->findMaxT();
        }
        else if (node->getParent())
        {
            while (node->getParent() && node->getParent()->getLeft() == node)
            {
                node = node->getParent();
            }
            if (node->getParent()) 
            {
                node = node->getParent();
            }
            else
            {
                node = nullptr; 
            }
        }
        else
        {
            node = nullptr;
        }

        return *this;
    }

    TreeIterator operator--(int) {
        TreeIterator old(node);
        --* this;
        return old;
    }
};
void testAddNode() 
{
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
	
	// Малый правый
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

    	// Большой левый
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


    	// Большой правый
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

void testdelete()
{
    	Tree* result = new Tree();
    	assert(result->delNode("GGT") == nullptr);
    	auto abc = result->addNode("Abc");
    	auto def = result->addNode("Def");
    	auto ghi = result->addNode("GHi"); 
    	auto aac = result->addNode("AAC");
    	auto aab = result->addNode("AAB");
    	auto gra = result->addNode("GRa");
    	auto gqa = result->addNode("GQA");

    	assert(result->delNode("GHi") == ghi);
    	assert(ghi->getParent() == nullptr);
    	assert(ghi->getLeft() == nullptr);
    	assert(ghi->getRight() == nullptr);
    	assert(gqa->getLeft() == nullptr);
    	assert(result->delNode("GGT") == nullptr);
    	assert(result->delNode("GQA") == gqa);
    	assert(gqa->getParent() == nullptr);
    	assert(gqa->getLeft() == nullptr);
    	assert(gqa->getRight() == nullptr);
    	assert(def->getRight() == gra);
    	assert(gra->getRight() == nullptr);
    	assert(gra->getLeft() == nullptr);
    	assert(gra->getParent() == def);
    	assert(result->delNode("AAC") == aac);
    	assert(aac->getRight() == nullptr);
}

void testiterator()
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

int main()
{
    	std::cerr << "Test" << std::endl;
    	testAddNode();
    	testdelete();
    	testiterator();
	return 0;
}
