#ifndef NODE_H
#define NODE_H

template<class T>
class Node {
  T data;
  Node<T> *next;

  public:
    Node(T d) :data(d), next(NULL) {}
    T get_data() { return data; }
    void set_data(T d) { data = d; }
    Node<T> *get_next() { return next; }
    void set_next(Node<T> * n) { next = n; }
    friend class LinkedList;
    friend class Queue;
};

#endif
