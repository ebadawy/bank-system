#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "Clerk.h"

class Customer {
  private:
    std::string name;
    std::string service;
    long service_time;
    long finish_time;
    Clerk *clerk;
  public:
    Customer();
    Customer(std::string n, std::string s);
    void set_name(std::string n);
    void set_service(std::string s);
    void set_clerk(Clerk *c);
    std::string get_name() { return name; }
    std::string get_service() { return service; }
    long get_service_time() { return service_time; }
    long get_finish_time() { return finish_time; }
    Clerk *get_clerk() { return clerk; }
    friend class Clerk;
};

Customer::Customer() {
  name = "";
  service = "";
  service_time = -1;
  finish_time = -1;
  clerk = NULL;
}

Customer::Customer(std::string n, std::string s) {
  name = n;
  service = s;
  service_time = -1;
  finish_time = -1;
  clerk = NULL; 
}

void Customer::set_name(std::string n) {
  name = n;
}

void Customer::set_service(std::string s) {
  service = s;
}

void Customer::set_clerk(Clerk *c) {
  clerk = c;
}

#endif
