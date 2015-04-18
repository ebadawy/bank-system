#ifndef CLERK_H
#define CLERK_H

#include <string>
#include "Customer.h"

class Customer;

class Clerk {
  private:
    std::string name;
    std::string customer;
    time_t arrival_time;
  public:
    Clerk();
    Clerk(std::string n, time_t t);
    void set_name(std::string n) { name = n; }
    void set_customer(std::string c) { customer = c; }
    void set_arrival_time(time_t t) { arrival_time = t; }
    time_t get_arrival_time() { return arrival_time; }
    std::string get_name() { return name; }
    std::string get_cutomer() { return customer; }
    friend class Customer;
};

Clerk::Clerk() {
  name = "";
  arrival_time = -1;
  customer = "";
}

Clerk::Clerk(std::string n, time_t t) {
  name = n;
  arrival_time = t;
  customer = "";
}

#endif
