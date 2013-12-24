//
//  HeaderItem.hpp
//  4710-Project-FrequentItemMining
//
//  Created by Brahmdeep Singh Juneja on 11/28/2013.
//  Copyright (c) 2013 Brahmdeep Singh Juneja. All rights reserved.
//

#ifndef _710_Project_FrequentItemMining_HeaderItem_hpp
#define _710_Project_FrequentItemMining_HeaderItem_hpp

#include "OrderedData.hpp"

using namespace std;

class FPTreeItem;
class FPTreeNode;
class NodeLL;

class HeaderItem : public OrderedData
{
private:
    FPTreeItem *data;
    float expSupport;
    
    FPTreeNode *firstSimilarTreeNode;
    NodeLL *firstPathNode;
    NodeLL *lastPathNode;
    
public:
    HeaderItem(FPTreeItem *data);
    virtual ~HeaderItem();
    void removeInfreqPathItems();
    
    void linkTreeNode(FPTreeNode *treeNode, HeaderItem *hash[]);
    void linkNextPath(NodeLL *path);
    
    int compareTo(OrderedData *other); //--refactor -
    void print();

    void increaseExpSuport(float probability);
    
    void increaseSupport();
    void increaseSupport(int inc);
    
    //************* GETTERS *************
    FPTreeItem* getData();
    FPTreeNode* getFirstSimilarTreeNode();
    NodeLL* getFirstPathNode();
    NodeLL* getLastPathNode();
    int getSimilarNodeCount();
    float getExpSupport();
    
    //************* SETTERS *************
    void setFirstSimilarTreeNode(FPTreeNode *firstSimilarTreeNode);
    void setFirstPathNode(NodeLL *firstPathNode);
    void setLastPathNode(NodeLL *lastPathNode);
    void setExpSupport(float expSupport);
    
};

#endif