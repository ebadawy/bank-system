#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "Clerk.h"

class Customer {
  private:
    std::string name;
    std::string service;
    time_t service_time;
    time_t finish_time;
    time_t arrival_time;
    Clerk* clerk;
  public:
    Customer();
    Customer(std::string n, std::string s, time_t arrival_t);
    void set_name(std::string n);
    void set_service(std::string s);
    void set_clerk(Clerk c) { clerk = &c; } 
    void set_arrival_time(time_t t);
    void set_service_time(time_t t);
    std::string get_name() { return name; }
    std::string get_service() { return service; }
    time_t get_service_time() { return service_time; }
    time_t get_finish_time() { return finish_time; }
    time_t get_arrival_time() { return finish_time; }
    Clerk* get_clerk() { return clerk; }
    Customer& operator=(const Customer& c);
    bool operator==(Customer &c) { return (name == c.get_name()); }
    friend class Clerk;
};

Customer::Customer() {
  name = "";
  service = "";
  service_time = -1;
  finish_time = -1;
  arrival_time = -1;
}

Customer::Customer(std::string n, std::string s, time_t arrival_t) {
  name = n;
  service = s;
  service_time = -1;
  finish_time = -1;
  arrival_time = arrival_t;
}

void Customer::set_name(std::string n) {
  name = n;
}

void Customer::set_service(std::string s) {
  service = s;
}

void Customer::set_arrival_time(time_t t) {
  arrival_time = t;
}

void Customer::set_service_time(time_t t) {
  service_time = t;
  if(service == "withdraw")
    finish_time = service_time + 35;
  else if(service == "depose")
    finish_time = service_time + 45;
  else if(service == "transfer")
    finish_time = service_time + 55;
}

void Clerk::set_customer(Customer c) {
  customer = &c;
}

#endif
