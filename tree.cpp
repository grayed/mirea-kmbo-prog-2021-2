#include <iostream>
#include<cassert>

using namespace std;

class Tree;
class Node
{
    Node *left, *right, *parent;
    int height,hl,hr;
    friend class Tree;
    friend class TreeIterator;

    public:
        
        Node* getLeft() { return left; }
        const Node* getLeft() const { return left; }
        Node* getRight() { return right; }
        const Node* getRight() const { return right; }
        Node* getParent() { return parent; }
        const Node* getParent() const { return parent; }
        int getHeight() {return height;}
        
        
        std::string name; //значение в узле
        std::string description;
        long price;

        Node(const std::string &name_, Node *parent_=nullptr):
            left(nullptr), right(nullptr), parent(parent_),name(name_),height(0),hl(0),hr(0)
        {}


        Node* findMin()
        {
            Node *node = this; //начало от текущего узла, с которого вызвано
            while (node->left)
                node = node->left;
            return node;
        }

        Node* findMax() 
        {
            Node *node = this;
            while (node->right)
                node = node->right;
            return node;
        }  

};

class TreeIterator : public std::iterator<std::input_iterator_tag, Node> {
    Node *node;

public:
    TreeIterator() : node(nullptr) {}
    TreeIterator(Node *node_) : node(node_) {}

    bool operator == (const TreeIterator &other) const { return node == other.node; }
    bool operator != (const TreeIterator &other) const { return node != other.node; }

    Node& operator * () { return *node; }
    const Node& operator * () const { return *node; }

    TreeIterator& operator++()  //обход с самого левого нижнего края
    {    // префиксный   ++it
        if (node->right!=nullptr)
        {   
            node=node->right;
            while(node->left!=nullptr)
                node=node->left;
        }
        else
        {    
            if (node->parent==nullptr) //если всего один узел
            {    
                node=nullptr;
                return *this;
            }
            
            while(node->parent->right==node) //пока в правом поддереве
            {
                if(node->right)
                   node->hr=node->right->height+1;
                if (node->left)
                    node->hl=node->left->height+1;
                
                node->height=(node->hl>node->hr)?node->hl:node->hr; 
                node=node->parent; //подъем из правого поддерева
            
                if (node->parent==nullptr)
                {
                    node=nullptr;
                    return *this;
                }
             
            }
            
            if (node->left)
                node->hl=node->left->height+1;

            if(node->right)
                node->hr=node->right->height+1;
               
            node->height=(node->hl>node->hr)?node->hl:node->hr; 
            node=node->parent;
                    
        }
        
        return *this;
    }
    
    TreeIterator operator++(int) {
        TreeIterator old(node);
        ++*this;
        return old;
    }

    TreeIterator& operator--() 
    { //обход с самого правого нижнего края
        if (node->left!=nullptr)
        {
            node=node->left;
            while(node->right!=nullptr)
                node=node->right;
        }

        else
        {
            if(node->parent==nullptr) //если один узел
            {
                node=nullptr;
                return *this;
            
            }

            else
            {
                if(node->parent->left==node)//если находимся в левом поддереве
                //для возврата в основание дерева
                {
                    node=node->parent;
                    if(node->parent==nullptr) //если мы пришли в основание дерева (самый первый узел)
                    //чтобы не был выполнен переход к следующему узлу, который nullptr
                    {
                        node=nullptr;
                        return *this;
                    }

                }


            }

            node=node->parent;
        }

        return *this;

    }
    TreeIterator operator--(int) {
        TreeIterator old(node);
        --*this;
        return old;
    }
};
class Tree
{
    public:
        Node *root;

        Tree():root(nullptr){}

       // Особенностью АВЛ-дерева является то, что оно является сбалансированным в следующем смысле: для любого узла 
       // дерева высота его правого поддерева отличается от высоты левого поддерева не более чем на единицу
       // Высота дерева — это длина самого длинного пути от узла к листу.

        Node* smallTurnRight(Node* p) // правый поворот вокруг p
        {  
            Node* q=p->left;
            
            p->left=q->right;
            q->right->parent=p;
            q->right=p;

            p->height=p->height-1;
            q->height=q->height+1;
            
            return q;
        
        }

        Node* smallTurnLeft(Node* q)
        {
            Node* p=q->right;
            q->right=p->left;
            p->left->parent=q;
            p->left=q;

            q->height=q->height-1;
            p->height=p->height+1;
            

            return p;
            
            
        }
        
        //bigTurnLeft, bigTurnRight

        Node* bigTurnRight(Node* p)
        {
            Node* q=p->right;  //сначада делаем малый правый поворот, потом малый левый поворот
            Node* s;
            Node* a;
            
           /* q->left=s->right;
            s->right->parent=q;
            s->right=q;
            p->right=s;
            s->parent->p;
            
            
            p->right=s->left;
            s->left->parent=p;
            s->left=p;
            p->parent=s;*/
            a=smallTurnRight(q);
            p->right=a;
            a->parent=p;

            s=smallTurnLeft(p);

            return s;

        }

        Node* bidTurnLeft(Node *q)
        {
            Node* p=q->left;
            Node* a;
            Node* s;

            a=smallTurnLeft(p); //сначала делаем малый левый поворот, потом малый правый поворот
            q->left=a;
            a->parent=q;
            s=smallTurnRight(q);
            return s;
        }

        Node* findNode(const std::string &name)
        {
            for(auto node=root; node; ) //инициализация есть, есть условие,инкремент счетчика внутри цикла
            //node->left, node->right это переход к другому узлу
            //поэтому инкремент не нужно указывать в цикле
            //условие node- пока указатель существует
            //как только указатель ни на что не указывает,тогда выход из цикла 
            //выход как только указатель будет являться нулевым
            //Язык C++ неявно преобразует nullptr в соответствующий тип указателя.
            //Таким образом, nullptr неявно преобразуется в указатель типа int, а затем значение nullptr присваивается ptr.
            {
                auto res=name.compare(node->name); //compare даст 0, если равны, меньше нуля, если меньше данного и больше, если больше данного
                if(res==0)
                    return node;
                if(res<0)
                   node=node->left;
                else 
                    node=node->right;
            }

            return nullptr;
        } 
        
        Node* addNode(const std::string &name) 
        {
            Node *closest = findClosest(name);
            if (closest && closest->name == name)
                return nullptr;
            
            if (closest==nullptr)//это если элемент больше самого большого в бинарном дереве
            {
                for(Node* n=root;n->right;)
                {
                    n=n->right;
                    closest=n;
                }

            }
            Node *newNode = new Node(name, closest);
            newNode->parent=closest;
            newNode->left=closest->left;
            closest->left=newNode;
            
            Node* n=closest;
            TreeIterator p_n(n);
            while(n->left!=nullptr)
            {
                n=n->left;
                p_n=n;
            }
            
            int count=0;

            while(n!=closest || count!=2)
            {
                ++p_n; //вызов итератора 
                //указывает на текущий узел
                *n=*(p_n);
                if (n==closest)
                    count++;
            }
            
            //Длина для closest справа и слева
            if(n->left && n->right)
                n->height=((n->left->height>n->right->height)? n->left->height:n->right->height)+1;
            
            else
                if(n->left==nullptr && n->right)
                    n->height=n->right->height+1;
                else
                    if(n->right==nullptr && n->left)
                        n->height=n->left->height+1;
                    else
                        n->height=0;
            
            
            if(abs(n->right->height-n->left->height)<=1)
                return newNode;
            else
            {
                if(n->right->height-n->left->height==2) //правое поддерево превышает
                {    
                    if((n->right->right->height-n->right->left->height)<0) //если левое поддерево дочернего узла больше правого
                    //Тогда большой правый поворот 
                        n->right=smallTurnRight(n->right);
                    return smallTurnLeft(n);//малый левый поворот,вместе с малым правым дает большой правый
                }
                else
                    if(n->right->height-n->left->height==-2) //левое поддерево превышает
                    {
                        if((n->left->right->height-n->left->left->height)>0) //если правое поддерево дочернего узла больше левого
                        //тогда большой левый поворот
                            n->left=smallTurnLeft(n->left);
                        return smallTurnRight(n);//малый правый поворот,вместе с малым левым дает большой левый
                    }


            }


        }
        

        //Ищет узел с таким же максимально близким справа ("большим") значением name
        Node* findClosest(const std::string &name)
        { 
            Node* max_node=nullptr;
            for(auto node=root; node;)
            {
                auto res=name.compare(node->name); 
                //compare даст 0, если строки равны, меньше нуля, если передаваемое значени меньше данного и больше нуля, если переданное больше данного
                if(res==0)
                    return node;
                if(res<0)
                {   
                   max_node=node;
                   node=node->left;
                }
                   
                else
                    node=node->right;
            }
            //if (max_node!=nullptr)
                return max_node;
            //return nullptr; //если число окажется больше самого большого в бинарном дереве

        } 

        void deleteNode(Node* node)
        {
            //возможны три случая
            Node *p=node->parent;
            //Удаляем лист

            if (node->left==NULL && node->right==NULL)
            {    
                if (p!=nullptr)
                {
                    if (p->left==node) //если лист слева от родителя
                        p->left=NULL;
                    if(p->right==node) //если лист справа от родителя
                        p->right=NULL;
                }
                return;
            }

            //Удаляем узел, у которого один дочерний узел
            if(node->left!=NULL && node->right==NULL || node->right!=NULL && node->left==NULL)
            {    
                
                if(p!=nullptr)
                {
                    if(node->right!=NULL) //если только правый дочерний у удаляемого
                    {
                        if (p->right==node) //если удаляемый элемент справа от родителя,то
                            p->right=node->right;
                        
                        else  //если удаляемый элемент слева от родителя,то
                            p->left=node->right;   
                            
                        node->right->parent=p; 
                    }

                    else //если только левый дочерний у удаляемого
                    {
                        if(p->right==node) //если удаляемый справа от родителя
                            p->right=node->left;
                        else //если удаляемый слева от родителя
                            p->left=node->left;

                        node->left->parent=p; 
                    }
                }

                else
                {
                    if(node->right!=NULL) //если только правый дочерний у удаляемого
                    
                        node->right->parent=nullptr; 

                    else //если только левый дочерний у удаляемого
                    
                        node->left->parent=nullptr; 
                    
                }

                return;
                

            }

            // Если у удаляемого узла два дочерних узла
            
            Node *next=nullptr; //нужным нам  элемент
            Node *current=node; //текущий
        
            Node *parn=node->parent;
            
            
            if (parn!=nullptr)
            {
                current=current->left;
                
                while (current!=NULL)
                {
                    next=current;
                    current=next->right;
                }
                
                Node *par=next->parent;
                par->right=nullptr;

                if (parn->left==node)
                    parn->left=next;
                else
                    parn->right=next;

                node->right->parent=next;
                node->left->parent=next;

                
                /*current=current->right;
                
                while (current!=NULL)
                {
                    next=current;
                    current=next->left;
                } */

            }

            else
            {
                current=current->left;
                
                while (current!=NULL)
                {
                    next=current;
                    current=next->right;
                }
                
                Node *par=next->parent;
                par->right=nullptr;

                node->right->parent=next;

            }
        
      //либо идти налево и брать самый правый,либо направо и брать самый левый 

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

        TreeIterator begin()
        {   
          Node* node=root;
          if(node==nullptr)
              return TreeIterator();
              
          while(node->left)
              node=node->left;

           return TreeIterator(node);

        }

        TreeIterator end()
        {
            return TreeIterator();

        }


}; 

void testAddNode_1() //интерфейсный тест(создаем дерево и вызываем addNode())
{
    Tree *test=new Tree();
    auto a=test->addNode("A");
    //в скобках указывается ожидаемый результат
    assert(a!=nullptr);
    assert(a->name=="A");
    assert(a->getLeft()==nullptr);
    assert(a->getRight()==nullptr);
    assert(a->getParent()==nullptr);
    assert(a->getHeight()==0);

    auto b=test->addNode("B");
    assert(b!=nullptr);
    assert(b->name=="B");
    assert(b->getParent()==a);
    assert(a->getLeft()==b);
    assert(a->getHeight()==1);
    assert(b->getHeight()==0);
    assert(b->getRight()==nullptr);
    assert(b->getLeft()==nullptr);

}
void testAddNode_2() //инвазивный тест (сами создаем дерево и вызываем addNode())
{
    Tree test;
    Node* first=new Node("5");
    Node* second=new Node("7",first);
    
    Node* third=test.addNode("3");

    assert(first!=nullptr);
    assert(second!=nullptr);
    assert(third!=nullptr);
    assert(first->getParent()==nullptr);
    assert(second->getParent()==first);
    assert(third->getParent()==first);
    assert(first->getRight()==second);
    assert(first->getLeft()==third);
    assert(first->getHeight()==1);

}


void testdeleteNode_1() //интерфейсный тест
{
    Tree *test=new Tree();
    auto d=test->addNode("D");
    auto a=test->addNode("A");
    auto e=test->addNode("E");

    test->deleteNode(d);

    assert(d==nullptr);
    assert(a->getParent()==nullptr);
    assert(a->getRight()==e);
    assert(e->getParent()==a);

}
void testdeleteNode_2() //инвазивный тест
{
    Tree test;
    
    Node* first=new Node("5");
    Node* second=new Node("7",first);
    Node* third= new Node("3",first);

    test.deleteNode(first);
    assert(first==nullptr);
    assert(third->getParent()==nullptr);
    assert(third->getRight()==second);
    assert(second->getParent()==third);

}

void testIterator_1()
{
    Tree *tree=new Tree(); //выделяется динамическая память типа Tree,new возвращает указатель
    auto a=tree->addNode("7");
    auto b=tree->addNode("9");
    auto c=tree->addNode("2");
    TreeIterator it(c);
    
    assert(it!=nullptr);
    assert(it==c);
    ++it;
    assert(it==a);
    ++it;
    assert(it==b);
    ++it;
    assert(it==a);

}

void testIterator_2()
{
    Tree tree;
    Tree test;
    Node* a=new Node("7");
    Node* b=new Node("9",a);
    Node* c=new Node("2",a);
    
    TreeIterator it(c);
    
    assert(it!=nullptr);
    assert(it==c);
    ++it;
    assert(it==a);
    ++it;
    assert(it==b);
    ++it;
    assert(it==a);

}


int main()
{ 
    testAddNode_1();
    testAddNode_2();
    testdeleteNode_1();
    testdeleteNode_2();
    testIterator_1();
    testIterator_2();



    return 0;
}



//ближайшее с конца алфавита 
// а б г д 
//чтобы найти в надо вернуть г


//Справа всегда значения большие, чем в узле, слева-меньшие 
//Правое поддерево содержит только те элементы, которые больше вершины, из которой оно берет начало
//Левое поддерево содержит только те элементы, которые меньше вершины, из которой оно берет начало
//     13
//    /  \
//   10    20
//  /  \   /  \
// 7   11  15  24

//Для каждого узла все узлы левого поддерева содержат меньшие значения, а все узлы правого поддерева-большие значения




