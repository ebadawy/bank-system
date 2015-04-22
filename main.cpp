#include <ncurses.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include "Queue.h"
#include "Customer.h"
#include "Clerk.h"
#include "LinkedList.h"

using namespace std;

bool str_equals(char str[], string s);
void *input(void*);
void *clock(void*);
void *sys(void*);
void print_scr(WINDOW *win, string str);
void print_line(string str, int &row);
Queue<Customer> wating_customers;
LinkedList<Customer> serving_customers;
Queue<Clerk> idel_clerks;
LinkedList<Clerk> busy_clerks;

int line = 0;

time_t now;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t print_line_mutex = PTHREAD_MUTEX_INITIALIZER;
WINDOW *general_win;

int main(int argc, char *argv[]) {
  pthread_t input_thread, clock_thread, sys_thread;

  initscr();
  cbreak();
  curs_set(0);

  int iret1 = pthread_create(&input_thread, NULL, &input, NULL);
  int iret2 = pthread_create(&clock_thread, NULL, &clock, NULL);
  int iret3 = pthread_create(&sys_thread, NULL, &sys, NULL);

  if(iret1 || iret2 || iret3)
    printw("ERROR!");
  pthread_join(input_thread, NULL);
    
  endwin();
  return 0;
}


bool str_equals(char str[], string s) {
  for(int i = 0; i < s.length(); i++)
    if(str[i] != s.at(i))
      return false;
  return !str[s.length()];
}

void *input(void*) {
  WINDOW *input_win;
  int max_x, max_y;
  stringstream stm;
  struct tm *tmp;
  while(true){
    getmaxyx(stdscr, max_y, max_x);
    input_win = newwin(1, max_x, max_y-1, 0);
    general_win = newwin(max_y-2, max_x, 1, 0);
    char str[70];
    print_scr(input_win, ">> ");
    wgetstr(input_win, str);
    print_scr(input_win, ">> ");
    stringstream cmd;
    stringstream name, service;
    
    cmd << str[0] << str[1] << str[2] << str[3];
    if(cmd.str() == "quit" && !str[4])
      break;
    else if(cmd.str() == "cust") {
      int i = 9;
      char c = str[i];
      name.str("");
      service.str("");
      //get customer name
      while(c != ',') {
        name << c;
        c = str[++i];          
      }
      //get customer service
      while(c) {
        if(c != ',' && c != ' ')
          service << c;
        c = str[++i];
      }
      if(service.str() != "withdraw" && service.str() != "depose" &&
         service.str() != "transfer" )
        print_line("Malformed Service!", line);
      else {
        print_line(name.str() + " want to make a " + service.str() + " operation.", line);
        Customer c(name.str(), service.str(), now);
        wating_customers.enqueue(c);
      }
    } else if(cmd.str() == "cler") {
      if(str[5] == '-') {
        stringstream stm;
        stm << str[6] << str[7] << str[8] << str[9];
        if(stm.str() == "idel") {
          if(idel_clerks.get_length() == 0 && busy_clerks.get_length() != 0)
            print_line("All clerks are busy serving other customers.", line);
          else if(idel_clerks.get_length() == 0 && busy_clerks.get_length() == 0)
            print_line("There is no clerks yet arrived.", line);
          else {
            stringstream clerks;
            Node<Clerk> *current_clerk_node = idel_clerks.get_first();
            while(current_clerk_node != NULL) {
              clerks << current_clerk_node->get_data().get_name() << " ";
              current_clerk_node = current_clerk_node->get_next();
            }
            print_line("Idel clerks: " + clerks.str() + ".", line);
          }
          
        } else if(stm.str() == "busy") {
          if(idel_clerks.get_length() != 0 && busy_clerks.get_length() == 0)
            print_line("All clerks are wating for customers to serve.", line);
          else if(idel_clerks.get_length() == 0 && busy_clerks.get_length() == 0)
            print_line("There is no clerks yet arrived.", line);
          else {
            stringstream clerks;
            Node<Clerk> *current_clerk_node = busy_clerks.get_first();
            while(current_clerk_node != NULL) {
              clerks << current_clerk_node->get_data().get_name() << " ";
              current_clerk_node = current_clerk_node->get_next();
            }
            print_line("busy clerks: " + clerks.str() + ".", line);
          }
          
        } else{}
      } else if(str[5] == ' '){
        int i = 6;
        char c = str[i];
        while(c) {
          name << c;
          c = str[++i];
        }
        Clerk k(name.str(), now);
        print_line(name.str() + " has arrived.", line);
        idel_clerks.enqueue(k);
      } else
        print_line("Can't resolve this command!", line); 
        
    } else 
      print_line("Can't resolve this command!", line); 
    
  }
}

void *clock(void*) {
  WINDOW *clock_win;
  int max_x, max_y;
  stringstream stm;
  struct tm *tmp;
  
  while(true) {
    getmaxyx(stdscr, max_y, max_x);
    clock_win = newwin(1, 14, 0, max_x-14);
    sleep(1);
    time(&now);
    tmp = localtime(&now);
    stm.str("");
    stm << tmp->tm_hour << " : " << tmp->tm_min << " : " << tmp->tm_sec;
    print_scr(clock_win, stm.str());
 }
}

void *sys(void*) {
  bool exit = false;
  stringstream stm;
  struct tm *tmp;
  while(true) {
    sleep(1);
    while(!wating_customers.is_empty() && !idel_clerks.is_empty()) {
      Customer c = wating_customers.dequeue();
      Clerk k = idel_clerks.dequeue();
      c.set_clerk(k);
      c.set_service_time(now);
      serving_customers.add(c);
      k.set_customer(c);
      busy_clerks.add(k);
      stm << k.get_name() << " is serving " << c.get_name();
      print_line(stm.str(), line);
    }
    Node<Customer>* current_customer_node = serving_customers.get_head();
    string customer_name, clerk_name;
    while(current_customer_node != NULL) {
      if(current_customer_node->get_data().get_finish_time() <= now) {
        print_line("finsihed", line);
        Clerk *current_clerk = current_customer_node->get_data().get_clerk();
        clerk_name = current_clerk->get_name();
        customer_name = current_customer_node->get_data().get_name();
        if(serving_customers.remove(current_customer_node->get_data()))
          print_line(customer_name + " is now leaving.", line);
        if(busy_clerks.remove(*current_clerk)) {
          print_line(clerk_name + " is wating for another customer.", line);
          idel_clerks.enqueue(*current_clerk);
        }
      }
        current_customer_node = current_customer_node->get_next();  
    }
  }  
}
void print_line(string msg, int &row) {
  pthread_mutex_lock( &print_line_mutex );
  WINDOW *tmp;
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  tmp = newwin(row+2, max_x, row+1, 0);
  const char *c = msg.c_str();
  pthread_mutex_lock( &mutex1 ); 
  wprintw(tmp, c);
  pthread_mutex_lock( &mutex1 );
  wrefresh(tmp);
  row++;
  pthread_mutex_unlock( &print_line_mutex );
}
void print_scr(WINDOW *win, string msg) {
  pthread_mutex_lock( &mutex1 );
  const char *c = msg.c_str();
  werase(win);  
  wprintw(win, c);
  wrefresh(win);
  pthread_mutex_unlock( &mutex1 );
}
