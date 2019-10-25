#include <cstdlib>
#include <iostream>
#include <cstring>
#define M 3

#define keyType int64_t

struct Node{
    keyType key[2*M];
    Node* child[2*M+1];
    int key_num;
    int child_num;
    Node* parent;
    Node()
    {
        key_num=0;
        child_num=1;
        memset(key,0,sizeof(key));
        memset(child,0,sizeof(child));
        parent=NULL;
    }
    int search(keyType e)
    {
        int i=0;
        while(i<key_num)
        {
            if(key[i]>e)
                return i-1;
            i++;
        }
        return key_num-1;
    }

    void key_insert(int pos,keyType e)
    {
        for(int i=key_num-1;i>=pos;i--)
            key[i+1]=key[i];
        key[pos]=e;
        //child[pos+1]=NULL;
        key_num++;
    }

    void child_insert(int pos,Node* e)
    {
        for(int i=child_num-1;i>=pos;i--)
            child[i+1]=child[i];
        child[pos]=e;
        //child[pos+1]=NULL;
        child_num++;
    }

    int key_remove(int pos)
    {
        if(pos<0||pos>=key_num)
        {
            std::cout<<"key range error\n";
            return 0;
        }
        int e=key[pos];
        for(int i=pos;i<key_num;i++)
            key[i]=key[i+1];
        key_num--;
        return e;
    }

    Node* child_remove(int pos)
    {
        if(pos<0||pos>=child_num)
        {
            std::cout<<"child range error\n";
            return 0;
        }
        Node* v=child[pos];
        for(int i=pos;i<child_num;i++)
            child[i]=child[i+1];
        child_num--;
        return v;
    }
};
