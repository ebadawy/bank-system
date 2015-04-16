class Customer {
  private:
    string name;
    string service;
    long service_time;
    long finish_time;
    Clerk *clerk;
  public:
    Customer();
    Customer(string n, string s);
    void set_name(string n);
    void set_service(string s);
    void set_clerk(Clerk *c);
    string get_name() { return name; }
    string get_service() { return service; }
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

Customer::Customer(string n, string s) {
  name = n;
  service = s;
  sertice_time = -1;
  finish_time = -1;
  clerk = NULL; 
}

void Customer::set_name(string name) {
  name = n;
}

void Customer::set_service(string s) {
  service = s;
}

viod Customer::set_clerk(Clerk *c) {
  clerk = c;
}
