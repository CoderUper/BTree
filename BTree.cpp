#include "BTree.h"
#include <set>
std::set<Node*> node_ptr;
BTree::BTree(Node* _root,int _num,Node* _hot):root(_root),num(_num),hot(_hot)
{
    root=create_node();
    hot=root;
    std::cout<<"a Btree is created!\n";
}

BTree::~BTree()
{
    if(root)
        release(root);
    root=NULL;
    std::cout<<"\na BTree is destroyed!\n";
}

//生成一个新的节点并初始化
Node* BTree::create_node()
{
    Node* node=new Node();
    if(node==NULL)
    {
        std::cout<<"memory allocation error\n";
        exit(1);
    }
    return node;
}

Node* BTree::search(keyType key)
{
    Node* v=root;
    while(v)
    {
        int pos=v->search(key);  //在root中找不大于key的第一个节点
        /*
        if(key==30)
        {
            std::cout<<v->key[0]<<std::endl;
            std::cout<<"in BTree search pos :"<<pos<<std::endl;
           
        }
        */
        if(pos>=0&&v->key[pos]==key) return v;
        hot=v;
        if(v->is_leaf) return NULL;
        v=v->child[pos+1];
    }
    return NULL;
}

bool BTree::insert(keyType key)
{
    Node* node=search(key);
    if(node) return false;
    node=hot;
    int pos=node->search(key);
    node->key_insert(pos+1,key);
    node->child_insert(pos+1,NULL);
    num++;
    SolveOverflow(node);
    //std::cout<<"insert successful\n";
    return true;
}

bool BTree::remove(keyType key)
{
    Node* node=search(key);
    if(!node) return false;  //没找到指定元素
    int pos=node->search(key);
    if(!node->is_leaf)   //若ndoe为非叶节点，找到其中序下的直接后继
    {
        Node* p=node->child[pos+1];
        while(!p->is_leaf) p=p->child[0];
        node->key[pos]=p->key[0];
        node=p;
        pos=0;
    }
    node->key_remove(pos);
    node->child_remove(pos);
    num--;
    SolveUnderflow(node);
    return true;
}

//处理上溢，采用分裂的方法
void BTree::SolveOverflow(Node* node)
{
    //判断是否需要上溢操作
    if(node->key_num<2*M) return;
    //存储要分裂的关键之
    keyType e=node->key[M];
    //生成新节点，并赋值
    Node* new_node=create_node();
    //判断用B+树还是B树分裂法
    if(node->is_leaf)
    {
        for(int i=0;i<M;i++)
        {
            new_node->key_insert(i,node->key_remove(M));
            new_node->child_insert(i,node->child_remove(M));
        }
        new_node->child[M]=node->child_remove(M);
        node->child_insert(M,new_node);
    }
    else
    {
        for(int i=0;i<M-1;i++)
        {
            new_node->key_insert(i,node->key_remove(M+1));
            new_node->child_insert(i,node->child_remove(M+1));
        }
        new_node->child[M-1]=node->child_remove(M+1);
        node->key_remove(M);
    }
    new_node->is_leaf=node->is_leaf;
    new_node->parent=node->parent;
    if(!new_node->is_leaf)
        for(int i=0;i<new_node->child_num;i++)
            new_node->child[i]->parent=new_node;
    //分裂的点是根节点
    if(node->parent==NULL)
    {
        //初始化新根节点
        Node* new_root=create_node();
        new_root->child[0]=node;
        new_root->child[1]=new_node;
        new_root->key[0]=e;
        new_root->key_num=1;
        new_root->child_num=2;
        new_root->is_leaf=false;
        node->parent=new_root;
        new_node->parent=new_root;
        root=new_root;

    }
    //分裂点不是根节点
    else
    {
        Node* p=node->parent;
        int pos=p->search(e);
        p->key_insert(pos+1,e);
        p->child_insert(pos+2,new_node);
        new_node->parent=p;
        SolveOverflow(p);
    }
}

void BTree::SolveUnderflow(Node* node)
{
    if(node->key_num>=M-1) return;
    Node *p=node->parent;
    //node是根节点,不受最小数目限制
    if(!p)
    {
        //如果根节点空
        if(node->key_num==0&&node->child[0])
        {
            root=node->child[0];
            root->parent=NULL;
            node->child[0]=NULL;
            release(node);
        }
        return;
    }
    int pos=0;
    while(p->child[pos]!=node) pos++;  
    //node不是根节点，分三种情况讨论
    //（1）node有左兄弟且数目足够
    if(pos>0&&p->child[pos-1]->key_num>M-1)
    {
        Node* lb=p->child[pos-1];
        node->key_insert(0,p->key[pos-1]);
        p->key[pos-1]=lb->key_remove(lb->key_num-1);
        node->child_insert(0,lb->child_remove(lb->child_num-1));
        if(node->child[0]) node->child[0]->parent=node;
        return;
    }
    //（2）node有右兄弟且数目足够
    if(pos<p->key_num&&p->child[pos+1]->key_num>M-1)
    {
        Node* rb=p->child[pos+1];
        node->key_insert(node->key_num,p->key[pos]);
        p->key[pos]=rb->key_remove(0);
        node->child_insert(node->child_num,rb->child_remove(0));
        if(node->child[node->child_num-1])
            node->child[node->child_num-1]->parent=node;
    }
    //（3）向父节点借
    //必有左兄弟
    if(pos>0)
    {
        Node* lb=p->child[pos-1];
        lb->key_insert(lb->key_num,p->key_remove(pos-1));
        p->child_remove(pos);
        lb->child_insert(lb->child_num,node->child_remove(0));
        if(lb->child[lb->child_num-1]) lb->child[lb->child_num-1]->parent=lb;
        while(node->key_num!=0)
        {
            lb->key_insert(lb->key_num,node->key_remove(0));
            lb->child_insert(lb->child_num,node->child_remove(0));
            if(lb->child[lb->child_num-1]) lb->child[lb->child_num-1]->parent=lb;
        }
        release(node);
    }
    //必有右兄弟,数据转入右兄弟
    else
    {
        Node* rb=p->child[pos+1];
        //父节点key转入rb;
        rb->key_insert(0,p->key_remove(pos));
        p->child_remove(pos);
        //node节点的数据从又向左转入rb;
        rb->child_insert(0,node->child_remove(node->child_num-1));
        if(rb->child[0]) rb->child[0]->parent=0;
        while(node->key_num!=0)
        {
            rb->key_insert(0,node->key_remove(node->key_num-1));
            rb->child_insert(0,node->child_remove(node->child_num-1));
            if(rb->child[0]) rb->child[0]->parent=0;
        }
        release(node);
    }
    SolveUnderflow(p);
}

void BTree::release(Node* root)
{
    if(root)
    {
        for(int i=0;i<root->child_num;i++)
        {
            if(!root->is_leaf)
            {
               release(root->child[i]);
            }
        }       
        delete root;
        root=NULL;
    }
}

void BTree::bfs_print()
{
    if(!root) return;
    //初始化队列
    std::cout<<"init\n";
    int flag=0;
    Node* queue[num];
    memset(queue,0,sizeof(queue));
    int front=0,rear=0;
    queue[rear++]=root;
    while(front<rear)
    {
        Node *p=queue[front++];
        /*
        if(node_ptr.count(p))
        {
            std::cout<<"重复出现指针"<<p<<std::endl;
            flag=1;    
        }*/
        std::cout<<"[";
        for(int i=0;i<p->key_num;i++)
        {
            std::cout<<p->key[i]<<" ";
        }
        std::cout<<"]  ";
        //if(flag) break;
        node_ptr.insert(p);
        std::cout<<"key num in root is"<<p->key_num<<std::endl;
        for(int i=0;i<p->child_num;i++)
        {
            if(!p->is_leaf)
            {
                queue[rear++]=p->child[i];                   
                std::cout<<p->child[i]<<" ";
            }
        }
        std::cout<<"\n----------------\n";
    }
}

int BTree::get_num()
{
    return num;
}

