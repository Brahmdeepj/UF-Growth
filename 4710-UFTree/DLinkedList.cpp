//
//  DLinkedList.cpp
//  4710-Project-FrequentItemMining
//
//  Created by Brahmdeep Singh Juneja on 12/12/2013.
//  Copyright (c) 2013 Brahmdeep Singh Juneja. All rights reserved.
//

#include "DLinkedList.hpp"

#include <iostream>

#include "OrderedData.hpp"
#include "NodeLL.hpp"

DLinkedList::DLinkedList(){
    this->head = NULL;
    this->tail = NULL;
    this->size = 0;
}

// Purpose: destroys the entire list and its contents
DLinkedList::~DLinkedList(){
    NodeLL *curr = this->head;
    NodeLL *next = NULL;
    
    while (curr != NULL){
        next = curr->getNext();
        delete(curr);
        curr = next;
    }
    
    head = NULL;
    tail = NULL;
}

NodeLL* DLinkedList::addToFront(OrderedData *data) {
    bool success = false;
    NodeLL *newNode=NULL;
    
    if (data != NULL){
        newNode = new NodeLL(data, NULL, NULL);
        
        if (this->head == NULL && this->tail == NULL){  //empty list
            this->head = newNode;
            this->tail = newNode;
        } else {
            newNode->setNext(this->head);
            this->head->setPrev(newNode);
            this->head = newNode;
        }
        
        success = true;
        this->size++;
    }
    return newNode;
}

NodeLL* DLinkedList::addToBack(OrderedData *data) {
    bool success = false;
    NodeLL *newNode=NULL;
    
    if (data != NULL){
        newNode = new NodeLL(data, NULL, NULL);
        
        if (this->head == NULL && this->tail == NULL){  //empty list
            this->head = newNode;
            this->tail = newNode;
        } else {
            newNode->setPrev(this->tail);
            this->tail->setNext(newNode);
            this->tail = newNode;
        }
        
        success = true;
        this->size++;
    }
    
    return newNode;
}

// Purpose: Iterates through to find an item which comparitively equals to the given target
// Parm   :
// Return : ptr to the node that contains an item equals to the given taget
NodeLL* DLinkedList::find(OrderedData *target){
    NodeLL *curr = this->head;
    
    while (curr != NULL && curr->getData()->compareTo(target) == 0){
        curr = curr->getNext();
    }
    
    return curr;
}

// Purpose: pops the node containing the item similar to target
OrderedData* DLinkedList::remove(OrderedData *target){
    NodeLL *targetNode = find(target);
    OrderedData *item = NULL;
    
    if (targetNode != NULL){
        
        //unlink targetNode
        if (targetNode == this->head && targetNode == this->tail){
            this->head = NULL;
            this->tail = NULL;
        } else if (targetNode == this->head){
            this->head = this->head->getNext();
            if (head != NULL){
                this->head->setPrev(NULL);
            }
        } else if (targetNode == this->tail){
            this->tail = this->tail->getPrev();
            if (this->tail != NULL){
                this->tail->setNext(NULL);
            }
        } else {
            targetNode->getPrev()->setNext(targetNode->getNext());
            targetNode->getNext()->setPrev(targetNode->getPrev());
        }
        
        //save data ptr
        item = targetNode->getData();
        
        //remove node
        targetNode->setData(NULL);
        delete(targetNode);
        this->size--;
    }
    return item;
}

void DLinkedList::remove(NodeLL *node)
{
    if(node != NULL)
    {
        NodeLL *currPrev = node->getPrev();
        NodeLL *currNext = node->getNext();
        
        if(node==head)
        {
            head = currNext;
            if(currNext!=NULL)
                currNext->setPrev(NULL);
        }else if(node==tail) {
            tail = currPrev;
            if(currPrev!=NULL)
                currPrev->setNext(NULL);
        }else {
            currPrev->setNext(currNext);
            currNext->setPrev(currPrev);
        }
        
        size-=1;
            
        delete node;
    }
}

void DLinkedList::destory(OrderedData *target)
{
    OrderedData *item = remove(target);
    
    if (item != NULL){
        delete(item);
    }
}

void DLinkedList::print(){
    NodeLL *curr = this->head;
    
    while (curr != NULL){
        curr->getData()->print();
        cout << endl;
        
        curr = curr->getNext();
    }
    cout << endl;
}

int DLinkedList::getSize()
{
    return this->size;
}

NodeLL* DLinkedList::getHead(){
    return this->head;
}