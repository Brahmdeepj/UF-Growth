//
//  HeaderTable.hpp
//  4710-Project-FrequentItemMining
//
//  Created by Brahmdeep Singh Juneja on 11/21/2013.
//  Copyright (c) 2013 Brahmdeep Singh Juneja. All rights reserved.
//

#ifndef _710_Project_FrequentItemMining_HeaderTable_hpp
#define _710_Project_FrequentItemMining_HeaderTable_hpp

#include <string>

#include "FPContants.hpp"

using namespace std;

class FPTreeItem;
class HeaderItem;
class HeaderItemList;
class NodeLL;

class HeaderTable {
    
private:    
    HeaderItemList *headerItems;
    int numDomainItems;
    float minSup;
    
    static bool populateHash(string fileName, HeaderItem *hash[MAX_DOMAIN_ITEMS]);
    static bool insertHeaderItems(HeaderTable *headerTable, HeaderItem *hash[MAX_DOMAIN_ITEMS]);
    static void increaseExpSupport(FPTreeItem *item, HeaderItem *hash[MAX_DOMAIN_ITEMS]);
    
public:
    HeaderTable(int minSup);
    virtual ~HeaderTable();
    
    HeaderItem* insertByExpSupport(int domainItem, int support, float probability, float baseProbability);
    bool createHeaderTable(string fileName, HeaderItem *hash[MAX_DOMAIN_ITEMS]);
    void removeInfrequent();
    void printTable();
    
    static int getHashIndex(FPTreeItem *item);
    
    //************* GETTERS ***************
    int getNumDomainItem();
    NodeLL* getLowestFreqNode();
    NodeLL* getHighestFreqNode();
    HeaderItem* getItem(FPTreeItem *item);
};

#endif