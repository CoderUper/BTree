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
    Node* node=new Node;
    if(node==NULL)
    {
        std::cout<<"memory allocation error\n";
        exit(1);
    }
    for(int i=0;i<2*M;i++)
    {
        node->key[i]=0;
        node->child[i]=NULL;
    }
    node->child[2*M]=NULL;
    node->num=0;
    node->parent=NULL;
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
        v=v->child[pos+1];
    }
    return NULL;
}

bool BTree::insert(keyType key)
{
    Node* node=search(key);
    if(node) return false;
    node=hot;
    //std::cout<<"node addr is"<<node<<std::endl;
    int pos=node->search(key);
    std::cout<<"search key:"<<key<<";pos :"<<pos<<"\n";
    /*
    if(key==30)
    std::cout<<"in Node search pos: "<<pos<<std::endl;
    */
    node->insert(pos+1,key);
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
    if(node->child[0])   //若ndoe为非叶节点，找到其中序下的直接后继
    {
        Node* p=node->child[pos+1];
        while(p->child[0]) p=p->child[0];
        node->key[pos]=p->key[0];
        node=p;
    }
    node->remove(0);
    num--;
    if(num<M-1)
        SolveUnderflow();
    return true;
}

//处理上溢，采用分裂的方法
void BTree::SolveOverflow(Node* node)
{
    //判断是否需要上溢操作
    if(node->num<2*M) return;
    //存储要分裂的关键之
    keyType e=node->key[M];
    //生成新节点，并赋值
    Node* new_node=create_node();
    for(int i=0;i<M-1;i++)
    {
        new_node->key[i]=node->key[M+i+1];
        new_node->child[i]=node->child[M+i+1];
        node->key[M+i+1]=0;
        node->child[M+i+1]=NULL;
    }
    node->key[M]=0;
    new_node->child[M-1]=node->child[2*M];
    node->child[2*M]=NULL;
    node->num=M;
    new_node->num=M-1;
    new_node->parent=node->parent;
    if(new_node->child[0])
    {
        for(int i=0;i<=new_node->num;i++)
        {
            if(new_node->child[i]==NULL)
                std::cout<<i<<"new_ndoe child is NULL\n";
            new_node->child[i]->parent=new_node;
               
        }
    }
    /*
    if(e==31)
    {
        for(int i=0;i<new_node->num;i++)
            std::cout<<new_node->key[i]<<" ";
        std::cout<<"\n";
    }*/
    //分裂的点是根节点
    if(node->parent==NULL)
    {
        //初始化新根节点
        std::cout<<"root split:"<<e<<"------\n";
        std::cout<<"node addr is:"<<node<<";new_node is"<<new_node<<"\n";
        Node* new_root=create_node();
        new_root->child[0]=node;
        new_root->child[1]=new_node;
        new_root->key[0]=e;
        new_root->num=1;
        node->parent=new_root;
        new_node->parent=new_root;
        root=new_root;
        std::cout<<"new_root is:"<<new_root<<";\n";
        for(int i=0;i<new_root->num;i++)
            std::cout<<new_root->key[i]<<" ";
        std::cout<<"\n-----------------------\n";
        for(int i=0;i<=new_root->num;i++)
            std::cout<<new_root->child[i]<<" ";
        std::cout<<"---------------\n";
    }
    //分裂点不是根节点
    else
    {
        std::cout<<"not root split:"<<e<<"------\n";
        std::cout<<"node addr is:"<<node<<";new_node is"<<new_node<<"\n";       
        Node* p=node->parent;
        int pos=p->search(e);
        p->insert(pos+1,e);
        p->child[pos+1]=node;
        p->child[pos+2]=new_node;
         node->parent=p;
        new_node->parent=p;
        std::cout<<"parent is:"<<p<<";\n";
        for(int i=0;i<p->num;i++)
            std::cout<<p->key[i]<<" ";
        std::cout<<"\n-----------------------\n";
        for(int i=0;i<=p->num;i++)
            std::cout<<p->child[i]<<" ";
        std::cout<<"---------------\n";
        SolveOverflow(p);
    }
}

void BTree::SolveUnderflow()
{
    std::cout<<"SolveUnderflow()\n";
}

void BTree::release(Node* root)
{
    if(root)
    {
        for(int i=0;i<=root->num;i++)
        {
            if(root->child[i]!=NULL)
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
    int flag=0;
    Node* queue[num];
    memset(queue,0,sizeof(queue));
    int front=0,rear=0;
    queue[rear++]=root;
    while(front<rear)
    {
        Node *p=queue[front++];
        if(node_ptr.count(p))
        {
            std::cout<<"重复出现指针"<<p<<std::endl;
            flag=1;    
        }
        std::cout<<"[";
        for(int i=0;i<p->num;i++)
        {
            std::cout<<p->key[i]<<" ";
        }
        std::cout<<"]  ";
        if(flag) break;
        node_ptr.insert(p);
        std::cout<<"key num in root is"<<p->num<<std::endl;
        for(int i=0;i<=p->num;i++)
        {
            if(p->child[i]!=NULL)
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

