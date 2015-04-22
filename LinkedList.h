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
    Node<T>* remove(T node_data);
    int get_length();
    Node<T>* get_first();
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
int LinkedList<T>::get_length() {
  return length;
}

template<class T>
Node<T>* LinkedList<T>::remove(T node_data) {
    //virtual bool remove(Node<T> *node);
  if(head->data == node_data) {
    Node<T> *tmp = head;
    head = head->next;
    delete tmp;
    length--;
    return head;
  }
  Node<T> *currentNode = head;
  while(currentNode->next != NULL) {
    if(currentNode->next->data == node_data) {
      Node<T> *tmp = currentNode->next;
      currentNode->next = tmp->next;
      delete tmp;
      length--;
      return tmp->next;
    }
    currentNode = currentNode->next;
  }
  return NULL;
}

template<class T>
Node<T>* LinkedList<T>::get_first() {
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
