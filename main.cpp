#include <ncurses.h>

int main(int argc, char *argv[]) {
  WINDOW *input_win;
  
  initscr();
  cbreak();

  refresh();
  int max_x, max_y;

  getmaxyx(stdscr, max_y, max_x);
  input_win = newwin(1, max_x, max_y-1, 0);
  
  wprintw(input_win, ">>");
  wrefresh(input_win);
  char str[70];
  wgetstr(input_win, str);
  werase(input_win);
  wprintw(input_win, ">>");
  wrefresh(input_win);
  printw("You Entered: %s", str);
  refresh();
  getch();
  endwin();
  return 0;
}
