#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include "Node.h"

using namespace std;

template<class T>
class LinkedList {
  private:
    Node<T> *head;
    int length;
   public:
    LinkedList();
    void add(T val);
    virtual bool remove(T node_data);
    int size();
    Node<T>* get_head();
    void remove_head();
};

template<class T>
LinkedList<T>::LinkedList() {
  head = NULL;
  length = 0;
}

template<class T>
void LinkedList<T>::add(T val) {
  Node<T> *node = new Node<T>(val);
  if(head == NULL)
    head = node;
  else {
    Node<T> *currentNode = head;
    while(currentNode->next != NULL)
      currentNode = currentNode->next;
    currentNode->next = node;
  }
  length++;
}

template<class T>
int LinkedList<T>::size() {
  return length;
}

template<class T>
bool LinkedList<T>::remove(T node_data) {
  if(head->data == node_data) {
    Node<T> *tmp = head;
    head = head->next;
    delete tmp;
    length--;
    return true;
  }
  Node<T> *currentNode = head;
  while(currentNode->next != NULL) {
    if(currentNode->next->data == node_data) {
      Node<T> *tmp = currentNode->next;
      currentNode->next = tmp->next;
      delete tmp;
      length--;
      return true;
    }
    currentNode = currentNode->next;
  }
  return false;
}

template<class T>
Node<T>* LinkedList<T>::get_head() {
  return head;
}

template<class T>
void LinkedList<T>::remove_head() {
  Node<T> *tmp = head;
  head = head->next;
  if(tmp != NULL)
    length--;
  delete tmp;
}

#endif
