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

Queue<Customer> wating_customers;
LinkedList<Customer> serving_customers;
Queue<Clerk> idel_clerks;
LinkedList<Clerk> busy_clerks;

time_t now;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
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
      int i = 8;
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
        print_scr(general_win, "Malformed Service!");
      else {
        Customer c(name.str(), service.str(), now);
        wating_customers.enqueue(c);
      }
    } else if(cmd.str() == "cler") {
      int i = 5;
      char c = str[i];
      while(c) {
        name << c;
        c = str[++i];
      }
      Clerk k(name.str(), now);
      idel_clerks.enqueue(k);
    }
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
      c.set_clerk(k.get_name());
      c.set_service_time(now);
      serving_customers.enqueue(c);
      k.set_customer(c.get_name());
      busy_clerks.enqueue(k);
      stm << k.get_name() << " is serving " << c.get_name();
      print_scr(general_win, stm.str());
    }
  }  
}

void print_scr(WINDOW *win, string msg) {
  pthread_mutex_lock( &mutex1 );
  const char *c = msg.c_str();
  werase(win);  
  wprintw(win, c);
  wrefresh(win);
  pthread_mutex_unlock( &mutex1 );
}
