#ifndef QUEUE_H
#define QUEUE_H

#include "Node.h"

template<class T>
class Queue {
  private:
    Node<T> *first;
    int length;
  public:
    Queue();
    void enqueue(T val);
    T dequeue();
    Node<T>* get_first() { return first; }
    int get_length();
    bool is_empty();
    Node<T>* find(T t);
    int index_of(T t);
};

template<class T>
Queue<T>::Queue() {
  first = NULL;
  length = 0;
}

template<class T>
void Queue<T>::enqueue(T val) {
  Node<T> *node = new Node<T>(val);
  if(first == NULL)
    first = node;
  else {
    Node<T> *currentNode = first;
    while(currentNode->next)
      currentNode = currentNode->next;
    currentNode->next = node;
  }  
  length++;
}

template<class T>
int Queue<T>::get_length() {
  return length;
}

template<class T>
T Queue<T>::dequeue() {
  Node<T> *tmp = first;
  first = first->get_next();
  length--;
  T returnVal = tmp->get_data();
  delete tmp;
  return returnVal;
}

template<class T>
bool Queue<T>::is_empty() {
  return (first == NULL);
}

template<class T>
Node<T>* Queue<T>::find(T t) {
  Node<T>* current_node = first;
  while(current_node != NULL) {
    if(current_node->get_data() == t)
      return current_node;
    current_node = current_node->get_next();
  }
  return NULL;
}

template<class T>
int Queue<T>::index_of(T t) {
  Node<T>* current_node = first;
  int i = 0;
  while(current_node != NULL) {
    if(current_node->get_data() == t)
      return i;
    i++;
    current_node = current_node->get_next();
  }
  return -1;
}


#endif  
