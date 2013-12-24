//
//  HeaderItemList.h
//  4710-Project-FrequentItemMining
//
//  Created by Sukhpreet Lotey on 2013-12-17.
//  Copyright (c) 2013 Brahmdeep Singh Juneja. All rights reserved.
//

#ifndef ___710_Project_FrequentItemMining__HeaderItemList__
#define ___710_Project_FrequentItemMining__HeaderItemList__

class HeaderItem;
class NodeLL;
class FPTreeItem;

class HeaderItemList
{
private:
    NodeLL* head;
    NodeLL* tail;
    NodeLL* extract(int item);
    void orderedInsert(NodeLL *item);
    
public:
    HeaderItemList();
    virtual ~HeaderItemList();
    
    HeaderItem* insertByExpSupport(HeaderItem* item);
    void orderedInsert(HeaderItem *item);
    void removeInfrequent(float minsup);
    void print();
    
    NodeLL* getHeadNode();
    NodeLL* getTailNode();
    HeaderItem* getItem(FPTreeItem *item);
    int getSize();
};
#endif
