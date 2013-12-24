//
//  FPTree.hpp
//  4710-Project-FrequentItemMining
//
//  Created by Brahmdeep Singh Juneja on 11/26/2013.
//  Copyright (c) 2013 Brahmdeep Singh Juneja. All rights reserved.
//

#ifndef _710_Project_FrequentItemMining_FPTree_hpp
#define _710_Project_FrequentItemMining_FPTree_hpp

#include <string>
#include <vector>

#include "FPContants.hpp"

using namespace std;

class HeaderItem;
class HeaderTable;
class FPTreeNode;
class FPTreeItem;
class DLinkedList;

class FPTree
{
private:
    HeaderTable *headerTable;
    FPTreeNode *root;
    float minSup;
    
    int partialPrefix; //default = -1, indicating intial tree
    FPTree *parentProj;
    
    static void sortByPriority(FPTreeItem **array, int size, HeaderItem *hash[MAX_DOMAIN_ITEMS]);
    
public :
    FPTree(float minSup);
    FPTree(float minSup, int partialLabel, FPTree *parentProj);
    virtual ~FPTree();
    
    void createHeaderTable(string fileName, HeaderItem *hash[MAX_DOMAIN_ITEMS]);
    void createTree(string fileName, HeaderItem *hash[MAX_DOMAIN_ITEMS]);
    
    void insertTransaction(FPTreeItem **buffer, int size, HeaderItem *hash[MAX_DOMAIN_ITEMS]);
    void insertTransaction(DLinkedList *transactionItems);

    int totalTreeNodes();
    bool isSinglePath();
    bool isEmpty();
    void printHeaderTable();
    string getLabelPrefix();
    vector<string> *getSinglePath();
    int getBaseLevel();
    
    float getMinSup();
    HeaderTable* getHeaderTable();
    
    void printTree();
};
#endif