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
    T get_first();
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
T Queue<T>::dequeue() {
  if(first == NULL)
    return -1;
  Node<T> *tmp = first;
  first = first->get_next();
  length--;
  T returnVal = tmp->get_data();
  delete tmp;
  return returnVal;
}

template<class T>
T Queue<T>::get_first() {
  return first->get_data();
}
#endif  
