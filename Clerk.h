#ifndef CLERK_H
#define CLERK_H

#include <string>
#include "Customer.h"

class Customer;

class Clerk {
  private:
    std::string name;
    Customer *customer;
  public:
    Clerk();
    Clerk(std::string n);
    void set_name(std::string n) { name = n; }
    void set_cutomer(Customer *c) { customer = c; }
    std::string get_name() { return name; }
    Customer *get_cutomer() { return customer; }
    friend class Customer;
};

Clerk::Clerk() {
  name = "";
  customer = NULL;
}

Clerk::Clerk(std::string n) {
  name = n;
  customer = NULL;
}

#endif
