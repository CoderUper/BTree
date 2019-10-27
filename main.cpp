#include "BTree.h"
#include <ctime>
#include <vector>
std::vector<keyType> key_list;
#define random(x) rand()%(x)


int main()
{
    BTree bt;
    std::cout<<sizeof(Node)<<"B"<<std::endl;
    keyType keys[]={17 ,113 ,58 ,10 ,23 ,191 ,34 ,73 ,84 ,34 ,85 ,178 ,154 ,165 ,193 ,170 ,125 ,18 };
    int count=int(sizeof(keys)/sizeof(keyType));
    for(int i=0;i<count;i++)
        bt.insert(keys[i]);
    keyType n=1;
    srand((int)time(0));
    while(n<=1000)
    {
        int e=random(200)+1;
        bt.insert(e);
        key_list.push_back(e);
        n++;
    }
    std::cout<<"key num is "<<bt.get_num()<<std::endl;
    bt.bfs_print();
//    bt.remove(58);
/*
    for(int i=0;i<count;i++){
        bt.remove(keys[i]);  
    }
    bt.bfs_print();
    */
    /*
    for(std::vector<keyType>::iterator it=key_list.begin();it!=key_list.end();it++)
    {
        std::cout<<*it<<" ";
    }*/
    return 0;
}

