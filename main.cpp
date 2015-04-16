#include <ncurses.h>
#include<iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <sstream>

using namespace std;

bool str_equals(char str[], string s);
void *input(void*);
void *clock(void*);
void print_scr(WINDOW *win, string str);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
  pthread_t input_thread, clock_thread;
 
  initscr();
  cbreak();

  int iret1 = pthread_create(&input_thread, NULL, &input, NULL);
  int iret2 = pthread_create(&clock_thread, NULL, &clock, NULL);

  if(iret1 || iret2)
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
  WINDOW *general_win;
  int max_x, max_y;
  stringstream stm;
  while(true){
    getmaxyx(stdscr, max_y, max_x);
    input_win = newwin(1, max_x, max_y-1, 0);
    general_win = newwin(max_y-2, max_x-8, 0, 0);
    char str[70];
    print_scr(input_win, ">>");
    wgetstr(input_win, str);
    print_scr(input_win, ">>");
    if(!str_equals(str,"quit")) {
      stm.str("");
      stm << "You Entered: " << str;
      print_scr(general_win, stm.str()); 

    } else
      break; 
  }
}

void *clock(void*) {
  WINDOW *clock_win;
  int max_x, max_y;
  int sec = 0, min = 0;
  curs_set(0);
  print_scr(clock_win, "00 : 00");
  curs_set(2);
  stringstream stm;
  stm << min << " : " << sec;
  while(true) {
    getmaxyx(stdscr, max_y, max_x);
    clock_win = newwin(1, 7, 0, max_x-7);
    curs_set(0);
    print_scr(clock_win, stm.str());
    curs_set(2);
    sleep(1);
    if(sec > 58 && min > 58) 
      min = sec = 0;
    else if (sec > 58) {
      min++;
      sec = 0;
    } else
        sec++;
    stm.str("");
    stm << min << " : " << sec;
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
