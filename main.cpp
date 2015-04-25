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
Queue<Customer> withdraw_customers;
Queue<Customer> depose_customers;
Queue<Customer> transfer_customers;

Queue<Clerk> idle_clerks;
Queue<Clerk> withdraw_clerks;
Queue<Clerk> depose_clerks;
Queue<Clerk> transfer_clerks;

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
      if (str[8] == '-') {
        stringstream stm;
        stm << str[9] << str[10] << str[11] << str[12] << str[13] << str[14];
        if(stm.str() == "served") {
          if(wating_customers.get_length() == 0 && 
             withdraw_customers.is_empty() && depose_customers.is_empty() &&
             transfer_customers.is_empty())
            print_line("There is no customers yet arrived.", line);
          else if(wating_customers.get_length() != 0 && 
             withdraw_customers.is_empty() && depose_customers.is_empty() &&
             transfer_customers.is_empty())
            print_line("All customers are wating.", line);
          else {
            stringstream customers;
            Node<Customer> *current_customer_node = withdraw_customers.get_first();
            while(current_customer_node != NULL) {
              customers << current_customer_node->get_data().get_name() << ", ";
              current_customer_node = current_customer_node->get_next();
            }
            current_customer_node = depose_customers.get_first();
            while(current_customer_node != NULL) {
              customers << current_customer_node->get_data().get_name() << ", ";
              current_customer_node = current_customer_node->get_next();
            }
            current_customer_node = transfer_customers.get_first();
            while(current_customer_node != NULL) {
              customers << current_customer_node->get_data().get_name() << ", ";
              current_customer_node = current_customer_node->get_next();
            }
            print_line("Served Customers: " + customers.str().substr(0, customers.str().length()-2) + ".", line);
          } 
        } else if(stm.str() == "wating") {
          if(wating_customers.is_empty() && 
             withdraw_customers.is_empty() && depose_customers.is_empty() &&
             transfer_customers.is_empty())
            print_line("There is no customers yet arrived.", line);
          else if(wating_customers.is_empty() && 
             (!withdraw_customers.is_empty() || depose_customers.is_empty() ||
             transfer_customers.is_empty()))
            print_line("All customers are currently served.", line);
          else {
            stringstream customers;
            Node<Customer> *current_customer_node = wating_customers.get_first();
            while(current_customer_node != NULL) {
              customers << current_customer_node->get_data().get_name() << ", ";
              current_customer_node = current_customer_node->get_next();
            }
            print_line("Wating Customers: " + customers.str().substr(0, customers.str().length()-2) + ".", line); 
          }
        } else
          print_line("Unknown option -- usage [customer-served] [customer-wating]", line);
      } else if (str[8] == ' ') {
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
      }
    } else if(cmd.str() == "cler") {
      if(str[5] == '-') {
        stringstream stm;
        stm << str[6] << str[7] << str[8] << str[9];
        if(stm.str() == "idle") {
          if(idle_clerks.get_length() == 0 &&
             (!withdraw_customers.is_empty() || !depose_customers.is_empty() || 
              !transfer_customers.is_empty()))
            print_line("All clerks are busy serving other customers.", line);
          else if(idle_clerks.get_length() == 0 && 
                  withdraw_customers.is_empty() && depose_customers.is_empty() && 
                  transfer_customers.is_empty())
            print_line("There is no clerks yet arrived.", line);
          else {
            stringstream clerks;
            Node<Clerk> *current_clerk_node = idle_clerks.get_first();
            while(current_clerk_node != NULL) {
              clerks << current_clerk_node->get_data().get_name() << ", ";
              current_clerk_node = current_clerk_node->get_next();
            }
            print_line("Idle Clerks: " + clerks.str().substr(0,clerks.str().length()-2) + ".", line);
          }
          
        } else if(stm.str() == "busy") {
          if( idle_clerks.get_length() != 0 &&
              withdraw_customers.is_empty() && depose_customers.is_empty() && 
              transfer_customers.is_empty())
            print_line("All clerks are wating for customers to serve.", line);
          else if(idle_clerks.get_length() == 0 &&
                  withdraw_customers.is_empty() && depose_customers.is_empty() && 
                  transfer_customers.is_empty())
            print_line("There is no clerks yet arrived.", line);
          else {
            stringstream clerks;
              
            Node<Clerk> *current_clerk_node = withdraw_clerks.get_first();
            
            while(current_clerk_node != NULL) {
              clerks << current_clerk_node->get_data().get_name() << ", ";
              current_clerk_node = current_clerk_node->get_next();
            }
            
            current_clerk_node = depose_clerks.get_first();
            
            while(current_clerk_node != NULL) {
              clerks << current_clerk_node->get_data().get_name() << ", ";
              current_clerk_node = current_clerk_node->get_next();
            }
            
            current_clerk_node = transfer_clerks.get_first();
            
            while(current_clerk_node != NULL) {
              clerks << current_clerk_node->get_data().get_name() << ", ";
              current_clerk_node = current_clerk_node->get_next();
            }
            print_line("Busy Clerks: " + clerks.str().substr(0,clerks.str().length()-2) + ".", line);
          }
          
        } else
          print_line("Unknown option -- usage [clerk-busy] [clerk-idle]", line);
      } else if(str[5] == ' '){
        int i = 6;
        char c = str[i];
        while(c) {
          name << c;
          c = str[++i];
        }
        Clerk k(name.str(), now);
        print_line(name.str() + " has arrived.", line);
        idle_clerks.enqueue(k);
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
    while(!wating_customers.is_empty() && !idle_clerks.is_empty()) {
      Customer c = wating_customers.dequeue();
      Clerk k = idle_clerks.dequeue();
      c.set_clerk(k);
      c.set_service_time(now);
      k.set_customer(c);

      if(c.get_service() == "withdraw") {
        withdraw_customers.enqueue(c);
        withdraw_clerks.enqueue(k);
      } else if(c.get_service() == "depose") {
        depose_customers.enqueue(c);
        depose_clerks.enqueue(k);
      } else {
        transfer_customers.enqueue(c);
        transfer_clerks.enqueue(k);
      }

      stm.str("");
      stm << c.get_clerk()->get_name() << " is serving " << c.get_name() << ".";
      print_line(stm.str(), line);
    }

    if(!withdraw_customers.is_empty()&&
         withdraw_customers.get_first()->get_data().get_finish_time() <= now) {
        Customer c = withdraw_customers.dequeue();
        print_line(c.get_name() + " is now leaving.", line);
        Clerk k = withdraw_clerks.dequeue();
        idle_clerks.enqueue(k);
        print_line(k.get_name() + " is wating for another customer.", line);       
    }
    
    if(!depose_customers.is_empty()&&
        depose_customers.get_first()->get_data().get_finish_time() <= now) {
      Customer c = depose_customers.dequeue();
      print_line(c.get_name() + " is now leaving.", line);
      Clerk k = depose_clerks.dequeue();
      idle_clerks.enqueue(k);
      print_line(k.get_name() + " is wating for another customer.", line);
    }
    
    if(!transfer_customers.is_empty()&&
        transfer_customers.get_first()->get_data().get_finish_time() <= now) {
      Customer c = transfer_customers.dequeue();
      print_line(c.get_name() + " is now leaving.", line);
      Clerk k = transfer_clerks.dequeue();
      idle_clerks.enqueue(k);
      print_line(k.get_name() + " is wating for another customer.", line);
    }
  }  
}
void print_line(string msg, int &row) {
  pthread_mutex_lock( &mutex1 );
  WINDOW *tmp_win;
  struct tm *tmp;
  tmp = localtime(&now);
  stringstream str;
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  tmp_win = newwin(row+2, max_x, row+1, 0);
  int hour = tmp->tm_hour;
  int min = tmp->tm_min;
  int sec = tmp->tm_sec;
  if(hour <= 9)
    str << "[  " << hour;
  else
    str << "[ " << hour;;
  if(min <= 9) 
    str << " :  " << min;
  else
    str << " : " << min;
  if(sec <= 9)
    str << " :  " << sec << " ] " << msg;
  else
    str << " : " << sec << " ] " << msg;
  
  const char *c = str.str().c_str();
  wprintw(tmp_win, c);
  wrefresh(tmp_win);
  row++;
  pthread_mutex_unlock( &mutex1 );
}
void print_scr(WINDOW *win, string msg) {
  pthread_mutex_lock( &mutex1 );
  const char *c = msg.c_str();
  werase(win);  
  wprintw(win, c);
  wrefresh(win);
  pthread_mutex_unlock( &mutex1 );
}
