//
//  FPTreeNode.cpp
//  4710-Project-FrequentItemMining
//
//  Created by Brahmdeep Singh Juneja on 11/27/2013.
//  Copyright (c) 2013 Brahmdeep Singh Juneja. All rights reserved.
//

#include "FPTreeNode.hpp"

#include "FPContants.hpp"

#include "FPTreeItem.hpp"
#include "HeaderItem.hpp"
#include "HeaderTable.hpp"
#include "NodeLL.hpp"
#include "DLinkedList.hpp"
#include "TransPathItem.hpp"

//------------------------Constructors and destructors-----------------------
FPTreeNode::FPTreeNode(){
    this->data = NULL;
    this->nextSimilarNode = NULL;
    this->parent = NULL;
    this->nextSibling = NULL;
    this->headChild = NULL;
}
FPTreeNode::FPTreeNode(FPTreeItem *data, FPTreeNode *parent, FPTreeNode *nextSibling){
    this->data = data;
    this->parent = parent;
    this->nextSibling = nextSibling;
    
    this->nextSimilarNode = NULL;
    this->headChild = NULL;
}
FPTreeNode::~FPTreeNode(){
    if (data != NULL)
        delete(data);
    
    if (nextSibling != NULL)
        delete(nextSibling);
    if (headChild != NULL)
        delete(headChild);
}//--------------------------------------------------------------------------

/*-------------------------------------------------------------------------------------
 * PURPOSE: sequentially inserts the given items into 'this' node structure
 * PARM   : pre-sorted list of items (FPTreeItem *) that are to be inserted into
 *          current sub-tree
 * PARM   : hash[] for linking tree nodes
 * REMARKS: - items[] must be prioritized and contain only frequent items
 *-----------------------------------------------------------------------------------*/
void FPTreeNode::insertTransaction(FPTreeItem *items[MAX_DOMAIN_ITEMS], int size, HeaderItem *hash[MAX_DOMAIN_ITEMS]){
    this->insertTransactionItem(items, size, 0, hash); //recursive call
}

void FPTreeNode::insertTransaction(DLinkedList *items, HeaderTable *htable){
    this->insertTransactionItem(items->getHead(), htable); //recursive call
}

// PURPOSE: Recursively inserts each NodeLL's data (which should be of type FPTreeItem)
//          into node's sub-tree
void FPTreeNode::insertTransactionItem(FPTreeItem *items[MAX_DOMAIN_ITEMS], int size, int pos, HeaderItem *hash[MAX_DOMAIN_ITEMS])
{
    FPTreeNode *targetNode;
    
    if (pos >= 0 && pos < size && items[pos] != NULL){
        targetNode = this->insertChild(items[pos], hash); //insert currItem to subtree
        if (targetNode != NULL){
            items[pos] = NULL;
            targetNode->insertTransactionItem(items, size, pos+1, hash); //recursively insert next item
        }
    }
}

void FPTreeNode::insertTransactionItem(NodeLL *curr, HeaderTable *htable)
{
    FPTreeNode *targetNode;
    TransPathItem *item;
    
    if (curr != NULL && curr->getData() != NULL)
    {
        item = dynamic_cast<TransPathItem*>(curr->getData());
        if (item != NULL)
        {
            targetNode = this->insertChild(item->getItem(), NULL, htable);
            if (targetNode != NULL)
            {
                curr->setData(NULL);
                targetNode->insertTransactionItem(curr->getNext(), htable);
            }
        }
    }
}


/*-------------------------------------------------------------------------------------
 * PURPOSE: adds target as one of its children
 * PARM   : *target - item that is to be added as a child
 * PARM   : *headerTable - for used for linking new nodes to the table
 * RETURN : returns a ptr to the child that is added
 * REMARKS: if 'this' already contains a child similar to target, it increments the
 *          child's frequency instead of adding a new node
 *        : consumes given target
 *-----------------------------------------------------------------------------------*/
FPTreeNode* FPTreeNode::insertChild(FPTreeItem *target, HeaderItem *hash[MAX_DOMAIN_ITEMS]){
    FPTreeNode *targetNode = NULL;
    FPTreeNode *curr = this->headChild;
    FPTreeNode *prev = NULL;
    bool newNodeCreated = false;
    
    if (target != NULL && hash != NULL){
        //cycle to correct location
        while (curr != NULL && curr->getData() != NULL &&
               curr->compareTo(target) <= 0){
            prev = curr;
            curr = curr->getNextSibling();
        }
        
        if (prev == NULL){
            targetNode = new FPTreeNode(target, this, this->headChild);
            newNodeCreated = true;
            this->headChild = targetNode;
        } else {
            if (prev->isEqualsTo(target) == true){   //node already exists
                prev->data->increaseSupport(target);
                targetNode = prev;
            } else {
                targetNode = new FPTreeNode(target, this, curr);
                newNodeCreated = true;
                prev->nextSibling = targetNode;
            }
        }
        
        //add node link to header table if a new node was created
        if (newNodeCreated)
        {
            hash[HeaderTable::getHashIndex(target)]->linkTreeNode(targetNode, hash);
        } else {
            delete(target);
        }
    }
    return targetNode;
}

/*-------------------------------------------------------------------------------------
 * PURPOSE: adds target as one of its children
 * PARM   : *target - item that is to be added as a child
 * PARM   : *headerTable - for used for linking new nodes to the table
 * RETURN : returns a ptr to the child that is added
 * REMARKS: if 'this' already contains a child similar to target, it increments the
 *          child's frequency instead of adding a new node
 *        : consumes given target
 *-----------------------------------------------------------------------------------*/
FPTreeNode* FPTreeNode::insertChild(FPTreeItem *target, HeaderItem *hash[MAX_DOMAIN_ITEMS], HeaderTable *headerTable){
    FPTreeNode *targetNode = NULL;
    FPTreeNode *curr = this->headChild;
    FPTreeNode *prev = NULL;
    bool newNodeCreated = false;
    
    if (target != NULL){
        //cycle to correct location
        while (curr != NULL && curr->getData() != NULL &&
               curr->compareTo(target) <= 0){
            prev = curr;
            curr = curr->getNextSibling();
        }
        
        if (prev == NULL){
            targetNode = new FPTreeNode(target, this, this->headChild);
            newNodeCreated = true;
            this->headChild = targetNode;
        } else {
            if (prev->isEqualsTo(target) == true){   //node already exists
                prev->data->increaseSupport(target);
                targetNode = prev;
            } else {
                targetNode = new FPTreeNode(target, this, curr);
                newNodeCreated = true;
                prev->nextSibling = targetNode;
            }
        }
        
        //add node link to header table if a new node was created
        if (newNodeCreated)
        {
            if(headerTable != NULL)
            {
                FPTreeItem *temp = targetNode->getData();
                HeaderItem *headerItem = headerTable->getItem(temp);
                if(headerItem!=NULL) {
                    headerItem->linkTreeNode(targetNode, NULL);
                }
            }
            
        } else {
            delete(target);
        }
    }
    return targetNode;
}

// PURPOSE: counts total number of in its subtree, including itself
int FPTreeNode::countNodes(){
    int count = 1;
    
    if (this->nextSibling != NULL)
        count += this->nextSibling->countNodes();
    if (this->headChild != NULL)
        count += this->headChild->countNodes();
    
    return count;
}

//DEBUG - refactor
/*-------------------------------------------------------------------------------------
 * PURPOSE: compares based on alphabetical order and returns an integer representing
 *          result of comparison
 * PARM   : *OrderedItem , item that is to be compared
 * PARM   : 0  -> if both have similar data value
 *        : +1 -> current tree would appear after  the given, in a sorted order
 *        : -1 -> current tree swould appear before the given, in a sorted order
 *-----------------------------------------------------------------------------------*/
int FPTreeNode::compareTo(OrderedData *item){
    FPTreeNode *otherFPTreeNode = dynamic_cast<FPTreeNode *>(item);
    FPTreeItem *otherFPTreeItem = dynamic_cast<FPTreeItem *>(item);
    int result = 0;
    
    if (otherFPTreeNode != NULL)
    {
        result = this->data->getData() - otherFPTreeNode->data->getData();
        if (result == 0)
        {
            result = this->data->getProbability() - otherFPTreeNode->data->getProbability();
            
            if (result < 0)
                result = -1;
            else if (result > 0)
                result = 1;
        }
    } else if (otherFPTreeItem != NULL)
    {
        result = this->data->getData() - otherFPTreeItem->getData();
        
        result = this->data->getProbability() - otherFPTreeItem->getProbability();
        
        if (result < 0)
            result = -1;
        else if (result > 0)
            result = 1;
    }
    return result;
}

bool FPTreeNode::isEqualsTo(OrderedData *item){
    FPTreeNode *otherFPTreeNode = dynamic_cast<FPTreeNode *>(item);
    FPTreeItem *otherFPTreeItem = dynamic_cast<FPTreeItem *>(item);
    
    bool result = false;
    
    if (otherFPTreeNode != NULL)
        result = this->data->isEqualsTo(otherFPTreeNode->getData());
    else if (otherFPTreeItem != NULL)
        result = this->data->isEqualsTo(otherFPTreeItem);
    
    return result;
}

/*-------------------------------------------------------------------------------------
 * PURPOSE: prints the contents of the current node
 *-----------------------------------------------------------------------------------*/
void FPTreeNode::print(){
    this->data->print();
}

/*-------------------------------------------------------------------------------------
 * PURPOSE: prints the contents of node, subling, then children
 * PARM   : level - to identify pad space
 *-----------------------------------------------------------------------------------*/
void FPTreeNode::print(int level){
    stringstream ss;
    
    //pad '-'
    for (int i=0; i<level; i++){
        ss << "-";
    }
    
    //process self
    if (this->data == NULL){
        ss << "root";
    } else {
        ss << this->data->getData() << " : " << this->getData()->getSupport() ;
    }
    cout << ss.str() << endl;;
    
    //process children
    if (this->getHeadChild() != NULL){
        this->getHeadChild()->print(level+1);
    }
    
    //process sibling
    if (this->getNextSibling() != NULL){
        this->getNextSibling()->print(level);
    }
}

bool FPTreeNode::hasSingleChild()
{
    return (headChild==NULL || headChild->getNextSibling()==NULL);
}

//********************** GETTERS ***********************
FPTreeItem* FPTreeNode::getData(){ return this->data; }
FPTreeNode* FPTreeNode::getNextSimilarNode(){ return this->nextSimilarNode; }
FPTreeNode* FPTreeNode::getParent(){ return this->parent; }
FPTreeNode* FPTreeNode::getNextSibling(){ return this->nextSibling; }
FPTreeNode* FPTreeNode::getHeadChild(){ return this->headChild; }

//********************** Setters ***********************
void FPTreeNode::setNextSibling(FPTreeNode *nextSibling){
    this->nextSibling = nextSibling;
}

void FPTreeNode::setNextSimilarNode(FPTreeNode *nextSimilarNode){
    this->nextSimilarNode = nextSimilarNode;
}