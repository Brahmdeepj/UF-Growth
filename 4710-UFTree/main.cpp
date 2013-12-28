//
//  main.cpp
//
//  Created by Brahmdeep Singh Juneja on 11/21/2013.
//  Copyright (c) 2013 Brahmdeep Singh Juneja. All rights reserved.
//

#include "test.h"

#include <string>
#include <iostream>
#include <vector>

#include "FPTree.hpp"
#include "FPTreeNode.hpp"
#include "FPTreeItem.hpp"
#include "HeaderTable.hpp"
#include "HeaderItem.hpp"
#include "DLinkedList.hpp"
#include "TransPathItem.hpp"
#include "NodeLL.hpp"

using namespace std;
//-----------------------------------------------------------
// AUTHOR(S)	  :	Brahmdeep Singh Juneja, Sukhpreet Lotey
// COURSE		  : Comp 4710 - Data Mining
// INSTRUCTOR	  : Carson K. Leung
//
// REMARKS        : This implementation of an FP-Tree algorithm to mine data
//                  from a given data set.
//
//-----------------------------------------------------------
int k_level_itemset_counts[MAX_DOMAIN_ITEMS];

// private helper method for findFreqItems()
void getPrefix(int i, string &str, string &result)
{
    int count = 0;
    size_t pos = 0;
    size_t newPos = 0;
    std::string token;
    while ((newPos = str.find(',', pos)) != std::string::npos && count < i)
    {
        token = str.substr(pos, newPos-pos);
        pos=newPos+1;
        if(count == 0)
            result += token;
        else
            result += "," + token;
        
        count++;
    }
}

// finds all the permutations of a given single path items
void findFreqItems(int k, vector<string> *items, string label, int labelSize, int showFPs)
{
    vector<string> *fitems = new vector<string>;
    string formattedLabel;
    
    if (label != "")
        formattedLabel = label + ",";
    
    if(k==1)
    {
        for(int i=0; i<items->size(); i++)
        {
            string curri = items->at(i);
            
            //record - relative 1-itemset
            k_level_itemset_counts[labelSize+k-1]++;
            
            if (showFPs == 1)
                cout << "{" << formattedLabel << items->at(i) << "}" << endl;
            
            for (int j=i+1; j<items->size(); j++) {
                string currj = items->at(j);
                string joint = curri + "," + currj;
                fitems->push_back(joint);
            }
        }
        
    }else if(k>1) {
        for(int i=0; i<items->size(); i++)
        {
            string curri = items->at(i);
            string prefix;
            
            getPrefix(k-1, curri, prefix);
            
            int j = i+1;
            bool matches = true;
            while(matches && j<items->size())
            {
                string currj = items->at(j);
                string currjPrefix;
                getPrefix(k-1, currj, currjPrefix);
                
                if (prefix.compare(currjPrefix)==0)
                {
                    string joint = curri + "," + currj.substr(currjPrefix.length() + 1);
                    fitems->push_back(joint);
                } else
                {
                    matches = false;
                }
                
                j++;
            }
        }
    }
    
    //record - relative k-itemsets, where k>1
    for (int i=0; i<fitems->size(); i++)
    {
        k_level_itemset_counts[labelSize+k]++;
        
        if (showFPs == 1)
            cout << "{" << formattedLabel << fitems->at(i) << "}" << endl; //DEBUG
    }
    
    
    if (fitems->size()>1)
    {
        findFreqItems(k+1, fitems, label, labelSize, showFPs);
    }

}

void UFPGrowthMine(FPTree* tree, int showFPs, int countFPs)
{
    //record - relative 0-itemset
    if (tree->getBaseLevel() > 0 && tree->getLabelPrefix() != "")
    {
        k_level_itemset_counts[tree->getBaseLevel()-1]++;
        
        if (showFPs == 1)
            cout << "{" << tree->getLabelPrefix() << "}" << endl;
    }
    
    if(tree->isEmpty() || tree->isSinglePath())
    {
        vector<string> *singletons = tree->getSinglePath();
        findFreqItems(1, singletons, tree->getLabelPrefix(), tree->getBaseLevel(), showFPs);
    } else
    {
    	HeaderTable *headerTable = tree->getHeaderTable();
        
        // Iterates through each header item in the header table starting from
        // the bottom to the top
        NodeLL *currHeaderNode = dynamic_cast<NodeLL*>(headerTable->getLowestFreqNode());
        while(currHeaderNode != NULL)
        {
            HeaderItem *headerItem = (HeaderItem*)currHeaderNode->getData();
            DLinkedList *paths[headerItem->getSimilarNodeCount()];
            DLinkedList *path = NULL;
            
            FPTree *newProjTree = new FPTree(tree->getMinSup(), headerItem->getData()->getData(), tree);
            HeaderTable *projHeader = newProjTree->getHeaderTable();
            
            int currPathIndex = 0;
            
            // Iterates over nodes in the FP tree starting with a given header item
            // and generates a new list containing that path following the global
            // FP tree sorting order
            FPTreeNode *currSimilarNode = headerItem->getFirstSimilarTreeNode();
            while(currSimilarNode != NULL)
            {
                paths[currPathIndex] = new DLinkedList();
                path = paths[currPathIndex];
                
                int currNodeSupport = currSimilarNode->getData()->getSupport();
                float currNodeProb = currSimilarNode->getData()->getProbability();
                
                // generates a path by chasing the parent pointers
                FPTreeNode *parent = currSimilarNode->getParent();
                while(parent->getData()!=NULL)
                {
                    FPTreeItem *dataItem = parent->getData();
                    FPTreeItem *item = new FPTreeItem(dataItem->getData(), currNodeSupport, dataItem->getProbability());
                    TransPathItem *pathItem = new TransPathItem(item, paths[currPathIndex]);
                    NodeLL* addedPathNode = paths[currPathIndex]->addToFront(pathItem);
                    
                    // add to header table
                    HeaderItem *hItem = projHeader->insertByExpSupport(
                                                                       dataItem->getData(),
                                                                       currNodeSupport,
                                                                       dataItem->getProbability(),
                                                                       currNodeProb);
                    hItem->linkNextPath(addedPathNode);
                    
                    parent = parent->getParent();
                }// end - path generate loop
                
                currPathIndex++;
                
                currSimilarNode = currSimilarNode->getNextSimilarNode();
                
            } // end - all paths generation loop
            
            
//            // DEBUG Print
//            cout << "before pruning currHeaderItem " << headerItem->getData()->getData() << endl;
//            projHeader->printTable();
            
            // this removes all the infrequent items from the header table
            // while also filtering out the infrequents from the paths
            projHeader->removeInfrequent();
            
//            // DEBUG Print
//            cout << "after pruning currHeaderItem " << headerItem->getData()->getData() << endl;
//            projHeader->printTable();
            
            //insert all paths and DELETE them
            for (int i=0; i<headerItem->getSimilarNodeCount(); i++)
            {
                newProjTree->insertTransaction(paths[i]);
                
                // also DELETE existing path because that is no longer needed
                delete(paths[i]);
            }
            
//            // DEBUG Print
//            if(!newProjTree->isEmpty()) {
//    
//                cout << "\nItem: " << headerItem->getData()->getData() << " singlePath:" << newProjTree->isSinglePath() << endl ;
//                newProjTree->printTree();
//                cout << endl;
//            }
            
            UFPGrowthMine(newProjTree, showFPs, countFPs);
            
            // going backwards from lowest to highest order
            currHeaderNode = currHeaderNode->getPrev();
        }
    }
}

/*-----------------------------------------------------------------------------------
 * PURPOSE : primary handler method for mining a given datafile with indicated minsup
 * PARM    : string fileName, absolute file name
 * PARM    : int minSup, support threshold for frequent items
 *----------------------------------------------------------------------------------*/
FPTree* processFile(string fileName, float minSup)
{
    HeaderItem **hash =  new HeaderItem *[MAX_DOMAIN_ITEMS];
    FPTree *tree = new FPTree(minSup);
    
    //first pass - populate frequent items
    tree->createHeaderTable(fileName, hash); //will also fill hash[] with frequent 1-itemsets
    tree->printHeaderTable();
    
    //second pass - tree creation of frequent items
    if (tree->getHeaderTable()->getNumDomainItem() > 0) {
        tree->createTree(fileName, hash);
        tree->printTree();
        
        delete[] hash; //DEBUG - double check**
    }
    
    return tree;
}

int main(int argc, const char * argv[])
{
    clock_t start;
    double duration;
    
    if (argc == 7)
    {
        string fileName = argv[1];
        int minSup = atoi(argv[2]);
        
        int showFPs = atoi(argv[3]);
        int showTime = atoi(argv[4]);
        int countFPs = atoi(argv[5]);
        int countNodes = atoi(argv[6]);
        
        //**** DEBUG only ****
        showFPs = 0;
        showTime = 1;
        countFPs = 1;
        countNodes = 0;
        minSup = 0.5f;
        
        cout << "===== FP-growth TDB=" << fileName
        << " minsup=" << minSup
        << " showFPs=" << showFPs
        << " showTime=" << showTime
        << " countFPs=" << countFPs
        << " countNodes=" << countNodes
        << " =====" << endl;
        
        memset(k_level_itemset_counts, 0, (sizeof(int) * MAX_DOMAIN_ITEMS) );
        
        if (showFPs == 1)
            cout << "---------showFPs=" << showFPs << "-----------" << endl;
        
        //----------START---------
        start = clock();
        
        FPTree* globalTree = processFile(fileName, minSup); //read, create, and mine tree
        UFPGrowthMine(globalTree, showFPs, countFPs);
        
        //----------END-------
        duration = (clock() - start) / (double)  CLOCKS_PER_SEC;
        
        if (showFPs == 1)
            cout << endl;
        
        if (showTime == 1)
        {
            cout << "--------showTime=" << showTime << "----------" << endl;
            cout << "runtime = " << duration << endl << endl;
        }
        
        if (countFPs == 1)
        {
            cout << "--------countFPs=" << countFPs << "-----------" << endl;
            for (int i=0; i<MAX_DOMAIN_ITEMS && k_level_itemset_counts[i] != 0; i++)
                cout << "|L" << i+1 << "| = " << k_level_itemset_counts[i] << endl;
            cout << endl;
        }
        
        if (countNodes == 1)
        {
            cout << "------countNodes=" << countNodes << "----------"<< endl;
            cout << "total globalNodes = " << globalTree->totalTreeNodes() << endl;
            cout << endl;
        }
        
        cout << "===== FP-growth TDB=" << fileName
        << " minsup=" << minSup
        << " showFPs=" << showFPs
        << " showTime=" << showTime
        << " cntFPs=" << countFPs
        << " cntNodes=" << countNodes
        << ": Completed =====" << endl;
    } else
    {
        cout << "Invalid parameters "                                               << endl
        << "Usage: FPgrowth TDB minsup showFPs showTime cntFPs cntNodes"    << endl
        << "minsup   INTEGER"                                           << endl
        << "showFPs  0(No) or 1(Yes) to show all FPs with sup"          << endl
        << "showTime 0(No) or 1(Yes) to show runtime"                   << endl
        << "cntFPs   0(No) or 1(Yes) to count #FPs (k-itemsets for each k and total #FPs)" << endl
        << "cntNodes 0(No) or 1(Yes) to count #nodes"                   << endl
        << endl;
    }
    
    cout << endl << "End of processing." << endl;
    
    return EXIT_SUCCESS;
}
