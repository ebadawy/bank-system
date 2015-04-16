#include <ncurses.h>
#include<iostream>
#include <pthread.h>
#include <string>

using namespace std;

bool str_equals(char str[], string s);
void *input(void*);

int main(int argc, char *argv[]) {
  pthread_t t1;
 
  initscr();
  cbreak();

  int iret1 = pthread_create(&t1, NULL, &input, NULL);
  if(iret1)
    printw("Error");
  pthread_join(t1, NULL);
    
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

  while(true){
    getmaxyx(stdscr, max_y, max_x);
    input_win = newwin(1, max_x, max_y-1, 0);

    wprintw(input_win, ">>");
    wrefresh(input_win);
    char str[70];
    wgetstr(input_win, str);
    werase(input_win);
    wprintw(input_win, ">>");
    wrefresh(input_win);
    if(!str_equals(str,"quit")) {
      erase();
      printw("You Entered: %s", str);
      refresh();
    } else
      break; 
  }
}

