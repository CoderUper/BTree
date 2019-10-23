#include <cstdlib>
#define M 2

#define keyType int64_t

struct Node{
    keyType key[2*M];
    Node* child[2*M+1];
    int num;
    Node* parent;
    int search(keyType e)
    {
        int i=0;
        while(i<num)
        {
            if(key[i]>e)
                return i-1;
            i++;
        }
        return num-1;
    }

    void insert(int pos,keyType e)
    {
        for(int i=num-1;i>=pos;i--)
        {
            key[i+1]=key[i];
            child[i+2]=child[i+1];
        }
        key[pos]=e;
        //child[pos+1]=NULL;
        num++;
    }

    void remove(int pos)
    {
        for(int i=pos;i<num;i++)
        {
            key[i]=key[i+1];
            child[i]=child[i+1];
        }
    }
};
