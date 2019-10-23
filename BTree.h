#pragma once
#include "struct.h"
#include <iostream>
#include <cstring>
class BTree
{
public:
    BTree(Node* _root=NULL,int _num=0,Node* hot=NULL);
    ~BTree();
    Node* search(keyType key);
    Node* create_node();
    bool insert(keyType key);
    bool remove(keyType key);
    void SolveOverflow(Node* node);
    void SolveUnderflow();
    void release(Node * root);
    void bfs_print();
    int get_num();
private:
    Node* root;
    int num;
    Node* hot;     //指向当前节点
};

