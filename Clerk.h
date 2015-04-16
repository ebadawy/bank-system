#ifndef CLERK_H
#define CLERK_H

class Clerk {
  private:
    string name;
    Cutomer *customer;
  public:
    Clerk();
    Clerk(string n);
    void set_name(string n) { name = n; }
    void set_cutomer(Customer *c) { customer = c; }
    string get_name() { return name; }
    Customer *get_cutomer() { return customer; }
    freind class Customer;
};

Clerk::Clerk() {
  name = "";
  customer = NULL;
}

Clerk::Clerk(string n) {
  name = n;
  cutomer = NULL;
}

#endif
