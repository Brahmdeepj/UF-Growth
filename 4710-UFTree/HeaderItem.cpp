//
//  HeaderItem.cpp
//  4710-Project-FrequentItemMining
//
//  Created by Brahmdeep Singh Juneja on 11/28/2013.
//  Copyright (c) 2013 Brahmdeep Singh Juneja. All rights reserved.
//
//
//  HeaderItem.cpp
//  4710-Project-FrequentItemMining
//
//  Created by Brahmdeep Singh Juneja on 11/28/2013.
//  Copyright (c) 2013 Brahmdeep Singh Juneja. All rights reserved.
//

#include "FPTreeItem.hpp"
#include "HeaderItem.hpp"
#include "HeaderTable.hpp"
#include "FPTreeNode.hpp"
#include "NodeLL.hpp"
#include "TransPathItem.hpp"
#include "DLinkedList.hpp"

//------------------------Constructors and destructors-----------------------
HeaderItem::HeaderItem(FPTreeItem *data){
    this->data = data;
    this->firstSimilarTreeNode = NULL;
    this->firstPathNode = NULL;
    
    // must be multiplied by support when building paths from projected trees
    // for global header table - support is always 1 because paths are read from file
    this->expSupport = data->getProbability() * data->getSupport();
}

HeaderItem::HeaderItem(FPTreeItem *data, float baseProbability){
    this->data = data;
    this->firstSimilarTreeNode = NULL;
    this->firstPathNode = NULL;
    
    // must be multiplied by support when building paths from projected trees
    // for global header table - support is always 1 because paths are read from file
    this->expSupport = data->getProbability() * baseProbability * data->getSupport();
}

HeaderItem::~HeaderItem(){ //requires others to destroy path and tree nodes
    delete(data);
    this->data = NULL;
    this->firstPathNode = NULL;
    this->firstSimilarTreeNode = NULL;
}//--------------------------------------------------------------------------

void HeaderItem::removeInfreqPathItems()
{
    NodeLL *pathNode = firstPathNode;
    NodeLL *nextPath = NULL;
    TransPathItem *pathItem = NULL;
    DLinkedList *currPathList = NULL;
    
    while (pathNode!=NULL)
    {
        pathItem = (TransPathItem*)pathNode->getData();
        
//        //DEBUG Print
//        pathItem->print();
//        cout << "\n";

        currPathList = pathItem->getPathList();

        // get pointer to next path before destroying node
        nextPath = pathItem->getNextPathNode();
        
        // DEBUG not sure
        if(currPathList != NULL)
        {
            currPathList->remove(pathNode);
        }
                
        // dont delete because it is done by the currPathList itself
        //delete (pathNode);
        
        pathNode = nextPath;
    }
    //printf("\n");
}

/*-------------------------------------------------------------------------------------
 * PURPOSE: adds the link to the given node for the appropriate headerItem
 * PARM   : *node - that is to be linked to the header table
 * REMARKS: - this method links the node without question
 *-----------------------------------------------------------------------------------*/
void HeaderItem::linkTreeNode(FPTreeNode *treeNode, HeaderItem *hash[MAX_DOMAIN_ITEMS])
{
    FPTreeNode *curr = this->firstSimilarTreeNode;
    bool done = false;
    
    if(curr==NULL) {
        setFirstSimilarTreeNode(treeNode);
    }else {
        while (curr != NULL && !done)
        {
            if(curr->getNextSimilarNode()==NULL)
            {
                curr->setNextSimilarNode(treeNode);
                done = true;
            }
            
            if(!done)
                curr = curr->getNextSimilarNode();
        }
    }
}

void HeaderItem::linkNextPath(NodeLL *pathNodeToLink)
{
    if(this->firstPathNode==NULL) {
        firstPathNode = pathNodeToLink;
        lastPathNode = pathNodeToLink;
    }else {
        TransPathItem *pathItem = dynamic_cast<TransPathItem*>(lastPathNode->getData());
        pathItem->setNextPathNode(pathNodeToLink);
        lastPathNode = pathNodeToLink;
    }
}

/*-------------------------------------------------------------------------------------
 * PURPOSE: compares the frequency of two domain items 
 * PARM   : HeaderItem - item that is being compared
 * RETURN : <0 = 'this' item is less frequent
 *           0 = same item regardless of frequency
 *          >0 = 'this' item is more frequent
 * REMARKS: NULL items are taken as lowest priority
 *-----------------------------------------------------------------------------------*/
int HeaderItem::compareTo(OrderedData *other)
{
    HeaderItem *otherItem = dynamic_cast<HeaderItem *>(other);
    FPTreeItem *otherData = otherItem->getData();
    int result = 1;
    
    if (otherItem != NULL)
    {
        if (this->data->getData() == otherData->getData())
        {
            result = 0;
        } else if(this->getExpSupport() < otherItem->getExpSupport())
        {
            result = -1;
        } else if(this->getExpSupport() == otherItem->getExpSupport())
        {
            if (this->data->getData() < otherData->getData())
            {
                result = 1;
            } else
            {
                result = -1;
            }
        }
    }
    return result;
}

/*-------------------------------------------------------------------------------------
 * PURPOSE: prints priority, data value, frequency (e.g. "{10} 22 : 1")
 *-----------------------------------------------------------------------------------*/
void HeaderItem::print(){
    this->data->print();
    cout << " [" << this->expSupport << "]" << endl;
}

void HeaderItem::increaseExpSuport(float probability){
    this->expSupport += probability;
}

void HeaderItem::increaseSupport()
{
    if(data!=NULL)
    {
        data->increaseSupport();
    }
}

void HeaderItem::increaseSupport(int inc)
{
    if(data!=NULL)
    {
        data->increaseSupport(inc);
    }
}

//***************** GETTERS ******************
FPTreeItem* HeaderItem::getData()
{
    return this->data;
}
FPTreeNode* HeaderItem::getFirstSimilarTreeNode()
{
    return this->firstSimilarTreeNode;
}
NodeLL* HeaderItem::getFirstPathNode()
{
    return this->firstPathNode;
}

int HeaderItem::getSimilarNodeCount()
{
    int similarNodeCount = 0;
    
    FPTreeNode *curr = this->firstSimilarTreeNode;
    while (curr!=NULL) {
        similarNodeCount++;
        curr = curr->getNextSimilarNode();
    }
    
    return similarNodeCount;
}

NodeLL* HeaderItem::getLastPathNode()
{
    return lastPathNode;
}

float HeaderItem::getExpSupport()
{
    return expSupport;
}

//***************** SETTERS ******************
void HeaderItem::setFirstSimilarTreeNode(FPTreeNode *firstSimilarTreeNode)
{
    this->firstSimilarTreeNode = firstSimilarTreeNode;
}

void HeaderItem::setFirstPathNode(NodeLL *firstPathNode)
{
    this->firstPathNode = firstPathNode;
}

void HeaderItem::setLastPathNode(NodeLL *lastPathNode)
{
    this->lastPathNode = lastPathNode;
}