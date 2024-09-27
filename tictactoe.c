#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// there are 7 predefined colors, so I continue the initialization with 8
#define COLOR_BLUEISH 8
#define COLOR_REDISH 9
#define COLOR_GREY 10

void generateTIME(void) {
  time_t current_time;
  // I give the current time to the variable
  time(&current_time);
  struct tm *local_time = localtime(&current_time);
  if (local_time == NULL) {
    printf("eroare");
    return;
  }
  mvprintw(2, 50, "%02d/%02d/%d\n", local_time->tm_mday, local_time->tm_mon + 1,
           local_time->tm_year + 1900);
  mvprintw(2, 65, "%02d:%02d:%02d\n", local_time->tm_hour, local_time->tm_min,
           local_time->tm_sec);
  refresh();
}

void panel(void) {
  mvprintw(3, 50,
           "Be careful when using the click! It only works for certain areas.");
  mvprintw(4, 50, "It is the players decision whether to start with X or O.");
}

// I created the table for the game, drawing vertical and horizontal lines with
// the help of the functions in ncurses, then I created the setup for the mouse.
// Right click prints X, and left click prints O. I did this also with the help
// of the ncursed functions. I have set conditions for columns and rows, so that
// I have the possibility to print only on certain places. This function also
// finds out who the winner is.
void newgame(WINDOW *win, char mat[][100], int *score_x, int *score_o) {
  int ch, i, j, show = 0, start_x = 2, start_y = 2, width = 100, height = 100,
                right_was_clicked = 1, left_was_clicked = 1, cells = 0;
  // variable that stores information about mouse events
  MEVENT event;

  keypad(win, TRUE);
  // Set up mouse
  mousemask(ALL_MOUSE_EVENTS, NULL);

  if (!has_mouse()) {
    printw("Terminal does not support mouse");
    getch();
    return;
  }

  // I draw horizontal lines
  mvwhline(win, 2, 1, '-', 17);
  mvwhline(win, 5, 1, '-', 17);

  // I draw vertical lines
  mvwvline(win, 0, 6, '|', 8);
  mvwvline(win, 0, 12, '|', 8);

  if (!has_colors()) {
    printw("Terminal does not support color");
    getch();
    return;
  }
  start_color();

  // I create my own color, divide the RGB code, one by one, to 256
  init_color(COLOR_BLUEISH, 128, 0, 554);
  init_color(COLOR_REDISH, 488, 62, 160);
  init_color(COLOR_GREY, 156, 152, 152);

  // I create a pair of colors
  init_pair(1, COLOR_BLUEISH, COLOR_GREY);
  init_pair(2, COLOR_REDISH, COLOR_GREY);
  init_pair(3, COLOR_GREY, COLOR_GREY);

  wattron(win, COLOR_PAIR(3));
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 5; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 0; i < 2; i++) {
    for (j = 6; j < 11; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 0; i < 2; i++) {
    for (j = 12; j < 17; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 3; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 3; i < 5; i++) {
    for (j = 6; j < 11; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 3; i < 5; i++) {
    for (j = 12; j < 17; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 6; i < 8; i++) {
    for (j = 0; j < 5; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 6; i < 8; i++) {
    for (j = 6; j < 11; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 6; i < 8; i++) {
    for (j = 12; j < 17; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  while (1) {
    generateTIME();
    panel();

    ch = wgetch(win);
    if (ch == 'q') {
      break;
    } else {
      if (ch == KEY_MOUSE) {
        if (getmouse(&event) == OK) {
          if (((event.x - 2) == 3 || (event.x - 2) == 9 ||
               (event.x - 2) == 15) &&
              ((event.y - 2) == 1 || (event.y - 2) == 4 ||
               (event.y - 2) == 7)) {
            if ((event.bstate & BUTTON1_CLICKED) && right_was_clicked == 1) {
              right_was_clicked = 0;
              left_was_clicked = 1;
              wattron(win, COLOR_PAIR(1));
              mat[event.y - 2][event.x - 2] = 'X';
              mvwprintw(win, event.y - 2, event.x - 2, "X");
              wattroff(win, COLOR_PAIR(1));
              wrefresh(win);
            } else if ((event.bstate & BUTTON3_CLICKED) &&
                       left_was_clicked == 1) {
              right_was_clicked = 1;
              left_was_clicked = 0;
              wattron(win, COLOR_PAIR(2));
              mat[event.y - 2][event.x - 2] = 'O';
              mvwprintw(win, event.y - 2, event.x - 2, "O");
              wattroff(win, COLOR_PAIR(2));
              wrefresh(win);
            }
          }
        }
      }
    }

    // winner X on the main diagonal
    if (mat[1][3] == 'X' && mat[4][9] == 'X' && mat[7][15] == 'X') {
      (*score_x)++;
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      while (1) {
        mvprintw(1, 0,
                 " ___       __   ___  ________   ________   _______   "
                 "________          ___    ___ ");
        mvprintw(2, 0,
                 "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  ___ "
                 "\\ |\\   __  \\        |\\  \\  /  /|");
        mvprintw(3, 0,
                 "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\/  / /");
        mvprintw(4, 0,
                 " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\  \\_|/_\\ \\   _  _\\       \\ \\    / / ");
        mvprintw(5, 0,
                 "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                 " \\ \\  \\_|\\ \\ \\  \\\\  \\|       /     \\/  ");
        mvprintw(6, 0,
                 "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                 "\\__\\ \\_______\\ \\__\\ \\_\\      /  /\\   \\  ");
        mvprintw(7, 0,
                 "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                 "\\|__|\\|_______|\\|__|\\|__|    /__/ /\\ __\\ ");
        mvprintw(8, 0,
                 "                                                             "
                 "         |__|/ \\|__| ");

        refresh();

        for (i = 1; i <= 7; i += 3) {
          for (j = 3; j <= 15; j += 6) {
            mat[i][j] = ' ';
          }
        }

        ch = getch();
        if (ch == 'q') {
          return;
        }
      }
    }

    // winner O on the main diagonal
    if (mat[1][3] == 'O' && mat[4][9] == 'O' && mat[7][15] == 'O') {
      (*score_o)++;
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      while (1) {
        mvprintw(0, 0,
                 " ___       __   ___  ________   ________   _______   "
                 "________          ________     ");
        mvprintw(1, 0,
                 "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  ___ "
                 "\\ |\\   __  \\        |\\   __  \\    ");
        mvprintw(2, 0,
                 "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\|\\  \\   ");
        mvprintw(3, 0,
                 " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\  \\_|/_\\ \\   _  _\\       \\ \\  \\\\\\  \\  ");
        mvprintw(4, 0,
                 "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                 " \\ \\  \\_|\\ \\ \\  \\\\  \\|       \\ \\  \\\\\\  \\ ");
        mvprintw(5, 0,
                 "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                 "\\__\\ \\_______\\ \\__\\\\ _\\        \\ \\_______\\ ");
        mvprintw(6, 0,
                 "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                 "\\|__|\\|_______|\\|__|\\|__|        \\|_______| ");

        refresh();

        for (i = 1; i <= 7; i += 3) {
          for (j = 3; j <= 15; j += 6) {
            mat[i][j] = ' ';
          }
        }

        ch = getch();
        if (ch == 'q') {
          return;
        }
      }
    }

    // winner X on the secondary diagonal
    if (mat[1][15] == 'X' && mat[4][9] == 'X' && mat[7][3] == 'X') {
      (*score_x)++;
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      while (1) {
        mvprintw(1, 0,
                 " ___       __   ___  ________   ________   _______   "
                 "________          ___    ___ ");
        mvprintw(2, 0,
                 "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  ___ "
                 "\\ |\\   __  \\        |\\  \\  /  /|");
        mvprintw(3, 0,
                 "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\/  / /");
        mvprintw(4, 0,
                 " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\  \\_|/_\\ \\   _  _\\       \\ \\    / / ");
        mvprintw(5, 0,
                 "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                 " \\ \\  \\_|\\ \\ \\  \\\\  \\|       /     \\/  ");
        mvprintw(6, 0,
                 "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                 "\\__\\ \\_______\\ \\__\\ \\_\\      /  /\\   \\  ");
        mvprintw(7, 0,
                 "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                 "\\|__|\\|_______|\\|__|\\|__|    /__/ /\\ __\\ ");
        mvprintw(8, 0,
                 "                                                             "
                 "         |__|/ \\|__| ");
        refresh();

        for (i = 1; i <= 7; i += 3) {
          for (j = 3; j <= 15; j += 6) {
            mat[i][j] = ' ';
          }
        }

        ch = getch();
        if (ch == 'q') {
          return;
        }
      }
    }

    // winner O on the secondary diagonal
    if (mat[1][15] == 'O' && mat[4][9] == 'O' && mat[7][3] == 'O') {
      (*score_o)++;
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      while (1) {
        mvprintw(0, 0,
                 " ___       __   ___  ________   ________   _______   "
                 "________          ________     ");
        mvprintw(1, 0,
                 "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  ___ "
                 "\\ |\\   __  \\        |\\   __  \\    ");
        mvprintw(2, 0,
                 "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\|\\  \\   ");
        mvprintw(3, 0,
                 " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\  \\_|/_\\ \\   _  _\\       \\ \\  \\\\\\  \\  ");
        mvprintw(4, 0,
                 "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                 " \\ \\  \\_|\\ \\ \\  \\\\  \\|       \\ \\  \\\\\\  \\ ");
        mvprintw(5, 0,
                 "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                 "\\__\\ \\_______\\ \\__\\\\ _\\        \\ \\_______\\ ");
        mvprintw(6, 0,
                 "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                 "\\|__|\\|_______|\\|__|\\|__|        \\|_______| ");
        refresh();

        for (i = 1; i <= 7; i += 3) {
          for (j = 3; j <= 15; j += 6) {
            mat[i][j] = ' ';
          }
        }

        ch = getch();
        if (ch == 'q') {
          return;
        }
      }
    }

    // winner X on the lines
    for (i = 1; i <= 7; i += 3) {
      for (j = 3; j <= 15; j += 6) {
        if (mat[i][j] == 'X') {
          show++;
        }
      }
      if (show == 3) {
        (*score_x)++;
        clear();
        win = newwin(0, 0, 0, 0);
        refresh();

        win = newwin(height, width, start_y, start_x);
        while (1) {
          mvprintw(1, 0,
                   " ___       __   ___  ________   ________   _______   "
                   "________          ___    ___ ");
          mvprintw(2, 0,
                   "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  "
                   "___ \\ |\\   __  \\        |\\  \\  /  /|");
          mvprintw(3, 0,
                   "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                   "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\/  / /");
          mvprintw(4, 0,
                   " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                   " \\ \\  \\_|/_\\ \\   _  _\\       \\ \\    / / ");
          mvprintw(5, 0,
                   "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ "
                   "\\  \\ \\  \\_|\\ \\ \\  \\\\  \\|       /     \\/  ");
          mvprintw(6, 0,
                   "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                   "\\__\\ \\_______\\ \\__\\ \\_\\      /  /\\   \\  ");
          mvprintw(7, 0,
                   "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                   "\\|__|\\|_______|\\|__|\\|__|    /__/ /\\ __\\ ");
          mvprintw(8, 0,
                   "                                                           "
                   "           |__|/ \\|__| ");
          refresh();

          for (i = 1; i <= 7; i += 3) {
            for (j = 3; j <= 15; j += 6) {
              mat[i][j] = ' ';
            }
          }

          ch = getch();
          if (ch == 'q') {
            return;
          }
        }
      }
      show = 0;
    }

    // winner O on the lines
    for (i = 1; i <= 7; i += 3) {
      for (j = 3; j <= 15; j += 6) {
        if (mat[i][j] == 'O') {
          show++;
        }
      }
      if (show == 3) {
        (*score_o)++;
        clear();
        win = newwin(0, 0, 0, 0);
        refresh();

        win = newwin(height, width, start_y, start_x);
        while (1) {
          mvprintw(0, 0,
                   " ___       __   ___  ________   ________   _______   "
                   "________          ________     ");
          mvprintw(1, 0,
                   "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  "
                   "___ \\ |\\   __  \\        |\\   __  \\    ");
          mvprintw(2, 0,
                   "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                   "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\|\\  \\   ");
          mvprintw(3, 0,
                   " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                   " \\ \\  \\_|/_\\ \\   _  _\\       \\ \\  \\\\\\  \\  ");
          mvprintw(
              4, 0,
              "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
              "\\ \\  \\_|\\ \\ \\  \\\\  \\|       \\ \\  \\\\\\  \\ ");
          mvprintw(5, 0,
                   "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                   "\\__\\ \\_______\\ \\__\\\\ _\\        \\ \\_______\\ ");
          mvprintw(6, 0,
                   "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                   "\\|__|\\|_______|\\|__|\\|__|        \\|_______| ");
          refresh();

          for (i = 1; i <= 7; i += 3) {
            for (j = 3; j <= 15; j += 6) {
              mat[i][j] = ' ';
            }
          }

          ch = getch();
          if (ch == 'q') {
            return;
          }
        }
      }
      show = 0;
    }

    // winner X on the column
    for (i = 3; i <= 15; i += 6) {
      for (j = 1; j <= 7; j += 3) {
        if (mat[j][i] == 'X') {
          show++;
        }
      }
      if (show == 3) {
        (*score_x)++;
        clear();
        win = newwin(0, 0, 0, 0);
        refresh();

        win = newwin(height, width, start_y, start_x);
        while (1) {
          mvprintw(1, 0,
                   " ___       __   ___  ________   ________   _______   "
                   "________          ___    ___ ");
          mvprintw(2, 0,
                   "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  "
                   "___ \\ |\\   __  \\        |\\  \\  /  /|");
          mvprintw(3, 0,
                   "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                   "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\/  / /");
          mvprintw(4, 0,
                   " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                   " \\ \\  \\_|/_\\ \\   _  _\\       \\ \\    / / ");
          mvprintw(5, 0,
                   "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ "
                   "\\  \\ \\  \\_|\\ \\ \\  \\\\  \\|       /     \\/  ");
          mvprintw(6, 0,
                   "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                   "\\__\\ \\_______\\ \\__\\ \\_\\      /  /\\   \\  ");
          mvprintw(7, 0,
                   "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                   "\\|__|\\|_______|\\|__|\\|__|    /__/ /\\ __\\ ");
          mvprintw(8, 0,
                   "                                                           "
                   "           |__|/ \\|__| ");
          refresh();

          for (i = 1; i <= 7; i += 3) {
            for (j = 3; j <= 15; j += 6) {
              mat[i][j] = ' ';
            }
          }

          ch = getch();
          if (ch == 'q') {
            return;
          }
        }
      }
      show = 0;
    }

    // winner O on the column
    for (i = 3; i <= 15; i += 6) {
      for (j = 1; j <= 7; j += 3) {
        if (mat[j][i] == 'O') {
          show++;
        }
      }
      if (show == 3) {
        (*score_o)++;
        clear();
        win = newwin(0, 0, 0, 0);
        refresh();

        win = newwin(height, width, start_y, start_x);
        while (1) {
          mvprintw(0, 0,
                   " ___       __   ___  ________   ________   _______   "
                   "________          ________     ");
          mvprintw(1, 0,
                   "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  "
                   "___ \\ |\\   __  \\        |\\   __  \\    ");
          mvprintw(2, 0,
                   "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                   "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\|\\  \\   ");
          mvprintw(3, 0,
                   " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                   " \\ \\  \\_|/_\\ \\   _  _\\       \\ \\  \\\\\\  \\  ");
          mvprintw(
              4, 0,
              "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
              "\\ \\  \\_|\\ \\ \\  \\\\  \\|       \\ \\  \\\\\\  \\ ");
          mvprintw(5, 0,
                   "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                   "\\__\\ \\_______\\ \\__\\\\ _\\        \\ \\_______\\ ");
          mvprintw(6, 0,
                   "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                   "\\|__|\\|_______|\\|__|\\|__|        \\|_______| ");
          refresh();

          for (i = 1; i <= 7; i += 3) {
            for (j = 3; j <= 15; j += 6) {
              mat[i][j] = ' ';
            }
          }

          ch = getch();
          if (ch == 'q') {
            return;
          }
        }
      }
      show = 0;
    }

    // testing if it's a draw
    for (i = 1; i <= 7; i += 3) {
      for (j = 3; j <= 15; j += 6) {
        if (mat[i][j] != ' ') {
          cells++;
        }
      }
    }
    if (cells == 9) {
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();
      win = newwin(height, width, start_y, start_x);

      while (1) {
        mvprintw(0, 0, " ________  ________  ________  ___       __      ");
        mvprintw(1, 0,
                 "|\\   ___ \\|\\   __  \\|\\   __  \\|\\  \\     |\\  \\    ");
        mvprintw(2, 0,
                 "\\ \\  \\_|\\ \\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\    \\ \\ "
                 " \\   ");
        mvprintw(
            3, 0,
            " \\ \\  \\ \\\\ \\ \\   _  _\\ \\   __  \\ \\  \\  __\\ \\  \\  ");
        mvprintw(4, 0,
                 "  \\ \\  \\_\\\\ \\ \\  \\\\  \\\\ \\  \\ \\  \\ \\  "
                 "\\|\\__\\_\\  \\ ");
        mvprintw(
            5, 0,
            "   \\ \\_______\\ \\__\\\\ _\\\\ \\__\\ \__\\ \\____________\\");
        mvprintw(6, 0,
                 "    \\|_______|\\|__|\\|__|\\|__|\\|__|\\|____________|");

        refresh();

        ch = getch();
        if (ch == 'q') {
          return;
        }
      }
    }
    cells = 0;
  }
}

void resume(WINDOW *win, char mat[][100], int *score_x, int *score_o) {
  int ch, i, j, show = 0, start_x = 2, start_y = 2, width = 100, height = 100,
                right_was_clicked = 1, left_was_clicked = 1, cells = 0,
                resume = 0;
  // variable that stores information about mouse events
  MEVENT event;

  keypad(win, TRUE);
  // Set up mouse
  mousemask(ALL_MOUSE_EVENTS, NULL);

  if (!has_mouse()) {
    printw("Terminal does not support mouse");
    getch();
    return;
  }

  // I draw horizontal lines
  mvwhline(win, 2, 1, '-', 17);
  mvwhline(win, 5, 1, '-', 17);

  // I draw vertical lines
  mvwvline(win, 0, 6, '|', 8);
  mvwvline(win, 0, 12, '|', 8);

  if (!has_colors()) {
    printw("Terminal does not support color");
    getch();
    return;
  }
  start_color();

  // I create my own color, divide the RGB code, one by one, to 256
  init_color(COLOR_BLUEISH, 128, 0, 554);
  init_color(COLOR_REDISH, 488, 62, 160);
  init_color(COLOR_GREY, 156, 152, 152);

  // I create a pair of colors
  init_pair(1, COLOR_BLUEISH, COLOR_GREY);
  init_pair(2, COLOR_REDISH, COLOR_GREY);
  init_pair(3, COLOR_GREY, COLOR_GREY);

  wattron(win, COLOR_PAIR(3));
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 5; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 0; i < 2; i++) {
    for (j = 6; j < 11; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 0; i < 2; i++) {
    for (j = 12; j < 17; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 3; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 3; i < 5; i++) {
    for (j = 6; j < 11; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 3; i < 5; i++) {
    for (j = 12; j < 17; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 6; i < 8; i++) {
    for (j = 0; j < 5; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 6; i < 8; i++) {
    for (j = 6; j < 11; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(3));
  for (i = 6; i < 8; i++) {
    for (j = 12; j < 17; j++) {
      mvwprintw(win, i, j + 1, " ");
    }
  }
  wattroff(win, COLOR_PAIR(3));

  while (1) {
    generateTIME();
    panel();

    if (resume == 0) {
      for (i = 1; i <= 7; i += 3) {
        for (j = 3; j <= 15; j += 6) {
          if (mat[i][j] == 'X') {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, i, j, "X");
            wattroff(win, COLOR_PAIR(1));
            wrefresh(win);
          } else if (mat[i][j] == 'O') {
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, i, j, "O");
            wattroff(win, COLOR_PAIR(2));
            wrefresh(win);
          }
        }
      }
      resume = 1;
    }

    ch = wgetch(win);
    if (ch == 'q') {
      break;
    } else {
      if (ch == KEY_MOUSE) {
        if (getmouse(&event) == OK) {
          if (((event.x - 2) == 3 || (event.x - 2) == 9 ||
               (event.x - 2) == 15) &&
              ((event.y - 2) == 1 || (event.y - 2) == 4 ||
               (event.y - 2) == 7)) {
            if ((event.bstate & BUTTON1_CLICKED) && right_was_clicked == 1) {
              right_was_clicked = 0;
              left_was_clicked = 1;
              wattron(win, COLOR_PAIR(1));
              mat[event.y - 2][event.x - 2] = 'X';
              mvwprintw(win, event.y - 2, event.x - 2, "X");
              wattroff(win, COLOR_PAIR(1));
              wrefresh(win);
            } else if ((event.bstate & BUTTON3_CLICKED) &&
                       left_was_clicked == 1) {
              right_was_clicked = 1;
              left_was_clicked = 0;
              wattron(win, COLOR_PAIR(2));
              mat[event.y - 2][event.x - 2] = 'O';
              mvwprintw(win, event.y - 2, event.x - 2, "O");
              wattroff(win, COLOR_PAIR(2));
              wrefresh(win);
            }
          }
        }
      }
    }

    // winner X on the main diagonal
    if (mat[1][3] == 'X' && mat[4][9] == 'X' && mat[7][15] == 'X') {
      (*score_x)++;
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      while (1) {
        mvprintw(1, 0,
                 " ___       __   ___  ________   ________   _______   "
                 "________          ___    ___ ");
        mvprintw(2, 0,
                 "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  ___ "
                 "\\ |\\   __  \\        |\\  \\  /  /|");
        mvprintw(3, 0,
                 "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\/  / /");
        mvprintw(4, 0,
                 " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\  \\_|/_\\ \\   _  _\\       \\ \\    / / ");
        mvprintw(5, 0,
                 "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                 " \\ \\  \\_|\\ \\ \\  \\\\  \\|       /     \\/  ");
        mvprintw(6, 0,
                 "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                 "\\__\\ \\_______\\ \\__\\ \\_\\      /  /\\   \\  ");
        mvprintw(7, 0,
                 "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                 "\\|__|\\|_______|\\|__|\\|__|    /__/ /\\ __\\ ");
        mvprintw(8, 0,
                 "                                                             "
                 "         |__|/ \\|__| ");

        refresh();

        for (i = 1; i <= 7; i += 3) {
          for (j = 3; j <= 15; j += 6) {
            mat[i][j] = ' ';
          }
        }

        ch = getch();
        if (ch == 'q') {
          return;
        }
      }
    }

    // winner O on the main diagonal
    if (mat[1][3] == 'O' && mat[4][9] == 'O' && mat[7][15] == 'O') {
      (*score_o)++;
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      while (1) {
        mvprintw(0, 0,
                 " ___       __   ___  ________   ________   _______   "
                 "________          ________     ");
        mvprintw(1, 0,
                 "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  ___ "
                 "\\ |\\   __  \\        |\\   __  \\    ");
        mvprintw(2, 0,
                 "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\|\\  \\   ");
        mvprintw(3, 0,
                 " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\  \\_|/_\\ \\   _  _\\       \\ \\  \\\\\\  \\  ");
        mvprintw(4, 0,
                 "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                 " \\ \\  \\_|\\ \\ \\  \\\\  \\|       \\ \\  \\\\\\  \\ ");
        mvprintw(5, 0,
                 "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                 "\\__\\ \\_______\\ \\__\\\\ _\\        \\ \\_______\\ ");
        mvprintw(6, 0,
                 "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                 "\\|__|\\|_______|\\|__|\\|__|        \\|_______| ");

        refresh();

        for (i = 1; i <= 7; i += 3) {
          for (j = 3; j <= 15; j += 6) {
            mat[i][j] = ' ';
          }
        }

        ch = getch();
        if (ch == 'q') {
          return;
        }
      }
    }

    // winner X on the secondary diagonal
    if (mat[1][15] == 'X' && mat[4][9] == 'X' && mat[7][3] == 'X') {
      (*score_x)++;
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      while (1) {
        mvprintw(1, 0,
                 " ___       __   ___  ________   ________   _______   "
                 "________          ___    ___ ");
        mvprintw(2, 0,
                 "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  ___ "
                 "\\ |\\   __  \\        |\\  \\  /  /|");
        mvprintw(3, 0,
                 "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\/  / /");
        mvprintw(4, 0,
                 " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\  \\_|/_\\ \\   _  _\\       \\ \\    / / ");
        mvprintw(5, 0,
                 "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                 " \\ \\  \\_|\\ \\ \\  \\\\  \\|       /     \\/  ");
        mvprintw(6, 0,
                 "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                 "\\__\\ \\_______\\ \\__\\ \\_\\      /  /\\   \\  ");
        mvprintw(7, 0,
                 "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                 "\\|__|\\|_______|\\|__|\\|__|    /__/ /\\ __\\ ");
        mvprintw(8, 0,
                 "                                                             "
                 "         |__|/ \\|__| ");
        refresh();

        for (i = 1; i <= 7; i += 3) {
          for (j = 3; j <= 15; j += 6) {
            mat[i][j] = ' ';
          }
        }

        ch = getch();
        if (ch == 'q') {
          return;
        }
      }
    }

    // winner O on the secondary diagonal
    if (mat[1][15] == 'O' && mat[4][9] == 'O' && mat[7][3] == 'O') {
      (*score_o)++;
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      while (1) {
        mvprintw(0, 0,
                 " ___       __   ___  ________   ________   _______   "
                 "________          ________     ");
        mvprintw(1, 0,
                 "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  ___ "
                 "\\ |\\   __  \\        |\\   __  \\    ");
        mvprintw(2, 0,
                 "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\|\\  \\   ");
        mvprintw(3, 0,
                 " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                 "\\ \\  \\_|/_\\ \\   _  _\\       \\ \\  \\\\\\  \\  ");
        mvprintw(4, 0,
                 "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                 " \\ \\  \\_|\\ \\ \\  \\\\  \\|       \\ \\  \\\\\\  \\ ");
        mvprintw(5, 0,
                 "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                 "\\__\\ \\_______\\ \\__\\\\ _\\        \\ \\_______\\ ");
        mvprintw(6, 0,
                 "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                 "\\|__|\\|_______|\\|__|\\|__|        \\|_______| ");
        refresh();

        for (i = 1; i <= 7; i += 3) {
          for (j = 3; j <= 15; j += 6) {
            mat[i][j] = ' ';
          }
        }

        ch = getch();
        if (ch == 'q') {
          return;
        }
      }
    }

    // winner X on the lines
    for (i = 1; i <= 7; i += 3) {
      for (j = 3; j <= 15; j += 6) {
        if (mat[i][j] == 'X') {
          show++;
        }
      }
      if (show == 3) {
        (*score_x)++;
        clear();
        win = newwin(0, 0, 0, 0);
        refresh();

        win = newwin(height, width, start_y, start_x);
        while (1) {
          mvprintw(1, 0,
                   " ___       __   ___  ________   ________   _______   "
                   "________          ___    ___ ");
          mvprintw(2, 0,
                   "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  "
                   "___ \\ |\\   __  \\        |\\  \\  /  /|");
          mvprintw(3, 0,
                   "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                   "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\/  / /");
          mvprintw(4, 0,
                   " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                   " \\ \\  \\_|/_\\ \\   _  _\\       \\ \\    / / ");
          mvprintw(5, 0,
                   "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ "
                   "\\  \\ \\  \\_|\\ \\ \\  \\\\  \\|       /     \\/  ");
          mvprintw(6, 0,
                   "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                   "\\__\\ \\_______\\ \\__\\ \\_\\      /  /\\   \\  ");
          mvprintw(7, 0,
                   "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                   "\\|__|\\|_______|\\|__|\\|__|    /__/ /\\ __\\ ");
          mvprintw(8, 0,
                   "                                                           "
                   "           |__|/ \\|__| ");
          refresh();

          for (i = 1; i <= 7; i += 3) {
            for (j = 3; j <= 15; j += 6) {
              mat[i][j] = ' ';
            }
          }

          ch = getch();
          if (ch == 'q') {
            return;
          }
        }
      }
      show = 0;
    }

    // winner O on the lines
    for (i = 1; i <= 7; i += 3) {
      for (j = 3; j <= 15; j += 6) {
        if (mat[i][j] == 'O') {
          show++;
        }
      }
      if (show == 3) {
        (*score_o)++;
        clear();
        win = newwin(0, 0, 0, 0);
        refresh();

        win = newwin(height, width, start_y, start_x);
        while (1) {
          mvprintw(0, 0,
                   " ___       __   ___  ________   ________   _______   "
                   "________          ________     ");
          mvprintw(1, 0,
                   "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  "
                   "___ \\ |\\   __  \\        |\\   __  \\    ");
          mvprintw(2, 0,
                   "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                   "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\|\\  \\   ");
          mvprintw(3, 0,
                   " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                   " \\ \\  \\_|/_\\ \\   _  _\\       \\ \\  \\\\\\  \\  ");
          mvprintw(
              4, 0,
              "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
              "\\ \\  \\_|\\ \\ \\  \\\\  \\|       \\ \\  \\\\\\  \\ ");
          mvprintw(5, 0,
                   "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                   "\\__\\ \\_______\\ \\__\\\\ _\\        \\ \\_______\\ ");
          mvprintw(6, 0,
                   "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                   "\\|__|\\|_______|\\|__|\\|__|        \\|_______| ");
          refresh();

          for (i = 1; i <= 7; i += 3) {
            for (j = 3; j <= 15; j += 6) {
              mat[i][j] = ' ';
            }
          }

          ch = getch();
          if (ch == 'q') {
            return;
          }
        }
      }
      show = 0;
    }

    // winner X on the column
    for (i = 3; i <= 15; i += 6) {
      for (j = 1; j <= 7; j += 3) {
        if (mat[j][i] == 'X') {
          show++;
        }
      }
      if (show == 3) {
        (*score_x)++;
        clear();
        win = newwin(0, 0, 0, 0);
        refresh();

        win = newwin(height, width, start_y, start_x);
        while (1) {
          mvprintw(1, 0,
                   " ___       __   ___  ________   ________   _______   "
                   "________          ___    ___ ");
          mvprintw(2, 0,
                   "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  "
                   "___ \\ |\\   __  \\        |\\  \\  /  /|");
          mvprintw(3, 0,
                   "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                   "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\/  / /");
          mvprintw(4, 0,
                   " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                   " \\ \\  \\_|/_\\ \\   _  _\\       \\ \\    / / ");
          mvprintw(5, 0,
                   "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ "
                   "\\  \\ \\  \\_|\\ \\ \\  \\\\  \\|       /     \\/  ");
          mvprintw(6, 0,
                   "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                   "\\__\\ \\_______\\ \\__\\ \\_\\      /  /\\   \\  ");
          mvprintw(7, 0,
                   "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                   "\\|__|\\|_______|\\|__|\\|__|    /__/ /\\ __\\ ");
          mvprintw(8, 0,
                   "                                                           "
                   "           |__|/ \\|__| ");
          refresh();

          for (i = 1; i <= 7; i += 3) {
            for (j = 3; j <= 15; j += 6) {
              mat[i][j] = ' ';
            }
          }

          ch = getch();
          if (ch == 'q') {
            return;
          }
        }
      }
      show = 0;
    }

    // winner O on the column
    for (i = 3; i <= 15; i += 6) {
      for (j = 1; j <= 7; j += 3) {
        if (mat[j][i] == 'O') {
          show++;
        }
      }
      if (show == 3) {
        (*score_o)++;
        clear();
        win = newwin(0, 0, 0, 0);
        refresh();

        win = newwin(height, width, start_y, start_x);
        while (1) {
          mvprintw(0, 0,
                   " ___       __   ___  ________   ________   _______   "
                   "________          ________     ");
          mvprintw(1, 0,
                   "|\\  \\     |\\  \\|\\  \\|\\   ___  \\|\\   ___  \\|\\  "
                   "___ \\ |\\   __  \\        |\\   __  \\    ");
          mvprintw(2, 0,
                   "\\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
                   "\\ \\   __/|\\ \\  \\|\\  \\       \\ \\  \\|\\  \\   ");
          mvprintw(3, 0,
                   " \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\ "
                   " \\ \\  \\_|/_\\ \\   _  _\\       \\ \\  \\\\\\  \\  ");
          mvprintw(
              4, 0,
              "  \\ \\  \\|\\__\\_\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  "
              "\\ \\  \\_|\\ \\ \\  \\\\  \\|       \\ \\  \\\\\\  \\ ");
          mvprintw(5, 0,
                   "   \\ \\____________\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ "
                   "\\__\\ \\_______\\ \\__\\\\ _\\        \\ \\_______\\ ");
          mvprintw(6, 0,
                   "    \\|____________|\\|__|\\|__| \\|__|\\|__| "
                   "\\|__|\\|_______|\\|__|\\|__|        \\|_______| ");
          refresh();

          for (i = 1; i <= 7; i += 3) {
            for (j = 3; j <= 15; j += 6) {
              mat[i][j] = ' ';
            }
          }

          ch = getch();
          if (ch == 'q') {
            return;
          }
        }
      }
      show = 0;
    }

    for (i = 1; i <= 7; i += 3) {
      for (j = 3; j <= 15; j += 6) {
        if (mat[i][j] != ' ') {
          cells++;
        }
      }
    }
    if (cells == 9) {
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();
      win = newwin(height, width, start_y, start_x);

      while (1) {
        mvprintw(0, 0, " ________  ________  ________  ___       __      ");
        mvprintw(1, 0,
                 "|\\   ___ \\|\\   __  \\|\\   __  \\|\\  \\     |\\  \\    ");
        mvprintw(2, 0,
                 "\\ \\  \\_|\\ \\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\    \\ \\ "
                 " \\   ");
        mvprintw(
            3, 0,
            " \\ \\  \\ \\\\ \\ \\   _  _\\ \\   __  \\ \\  \\  __\\ \\  \\  ");
        mvprintw(4, 0,
                 "  \\ \\  \\_\\\\ \\ \\  \\\\  \\\\ \\  \\ \\  \\ \\  "
                 "\\|\\__\\_\\  \\ ");
        mvprintw(
            5, 0,
            "   \\ \\_______\\ \\__\\\\ _\\\\ \\__\\ \__\\ \\____________\\");
        mvprintw(6, 0,
                 "    \\|_______|\\|__|\\|__|\\|__|\\|__|\\|____________|");

        refresh();
      }

      ch = getch();
      if (ch == 'q') {
        return;
      }
    }
    cells = 0;
  }
}

void score(WINDOW *win, int score_x, int score_o) {
  int ch;
  while (1) {
    if (score_x >= score_o) {
      mvprintw(2, 2, "Score X: %d", score_x);
      mvprintw(3, 2, "Score O: %d", score_o);
    } else {
      mvprintw(2, 2, "Score O: %d", score_o);
      mvprintw(3, 2, "Score X: %d", score_x);
    }
    refresh();

    ch = getch();
    if (ch == 'q') {
      return;
    }
  }
}

// I add highlight to the option or not
void print_menu(WINDOW *win, int *highlight, char main_menu[][20],
                int *choice) {
  int i, column = 50, row = 11;
  for (i = 0; i < *choice; i++) {
    if (i + 1 == *highlight) {
      wattron(win, A_REVERSE);
      mvwprintw(win, row, column, "%s", main_menu[i]);
      wattroff(win, A_REVERSE);
    } else {
      mvwprintw(win, row, column, "%s", main_menu[i]);
    }
    row += 2;
  }
  wrefresh(win);
}

// I print the menu and move up and down in the menu. When highlight == 1,
// means that he is on New Game and must go to Best Score, that's why it
// takes the value of choice, i.e. 4, otherwise I decrease highlight to go up to
// the menu, for example, from Resume I go to New Game. The same thing happens
// for case KEY_DOWN.
void menu(WINDOW *win, int *highlight, char main_menu[][20], int *choice,
          int *key) {
  int c;
  // to be able to use arrows
  keypad(win, TRUE);

  mvprintw(0, 0,
           " _________  ___  ________         _________   ________  ________   "
           "       _________   ________  _______      ");
  mvprintw(1, 0,
           "|\\___   ___\\\\  \\|\\   ____\\       |\\___   ___\\ \\ \  __  \\ "
           "\\   ____\\        |\\___   ___ \\    __  \\ \\  ___ \\     ");
  mvprintw(
      2, 0,
      "\\|___ \\  \\_\\ \\  \\ \\  \\___|        \\|___ \\  \\_\\\ \\  \\ \\  "
      "\\ \\   ___|        \\|___ \\  \\_| \\   \\ \\  \\ \\   __/|    ");
  mvprintw(
      3, 0,
      "     \\ \\  \\ \\ \\  \\ \\  \\                \\ \\  \\ \\ \\   __  \\ "
      "\\  \\                \\ \\  \\ \\ \\   \\ \\  \\ \\  \\_|/__  ");
  mvprintw(
      4, 0,
      "      \\ \\  \\ \\ \\  \\ \\  \\____            \\ \\  \\ \\ \\  \\ \\  "
      "\\ \\  \\____            \\ \\  \\ \\ \\   \\ \\  \\ \\  \\_|\\ \\ ");
  mvprintw(
      5, 0,
      "       \\ \\__\\ \\ \\__\\ \\_______\\           \\ \\__\\ \\ \\__\\ "
      "\\__\\ \\_______\\           \\ \\__\\ \\ \\________\\ \\_______\\ ");
  mvprintw(
      6, 0,
      "        \\|__|  \\|__|\\|_______|            \\|__|  "
      "\\|__|\\|__|\\|_______|            \\|__|  \\|________|\\|_______|");

  refresh();
  print_menu(win, highlight, main_menu, choice);

  while (1) {
    c = wgetch(win);

    switch (c) {
      case KEY_UP:
        if (*highlight == 1)
          *highlight = *choice;
        else
          (*highlight)--;
        break;
      case KEY_DOWN:
        if (*highlight == *choice)
          (*highlight) = 1;
        else
          (*highlight)++;
        break;
      case 10:
        *key = *highlight;
        break;
      default:
        break;
    }
    print_menu(win, highlight, main_menu, choice);
    if (*key != 0) break;
  }
}

int main(int argc, char const *argv[]) {
  int start_x = 2, start_y = 2, width = 100, height = 100, choice = 4,
      highlight = 1, key = 0, score_x = 0, score_o = 0, i, j;
  WINDOW *win;
  char mat[100][100];
  char main_menu[4][20] = {"NEW GAME", "RESUME", "QUIT", "BEST SCORE"};

  for (i = 1; i <= 7; i += 3) {
    for (j = 3; j <= 15; j += 6) {
      mat[i][j] = ' ';
    }
  }

  // ncursed mode initialization
  initscr();
  clear();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);

  // hide the cursor
  curs_set(0);

  while (1) {
    clear();

    // I create the empty window
    win = newwin(0, 0, 0, 0);
    refresh();

    start_x = 2;
    start_y = 2;
    win = newwin(height, width, start_y, start_x);
    wrefresh(win);

    // I generate the menu according to the options in the string array
    menu(win, &highlight, main_menu, &choice, &key);
    refresh();

    // New Game
    if (key == 1) {
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      newgame(win, mat, &score_x, &score_o);
      refresh();
    }

    // Resume
    if (key == 2) {
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      resume(win, mat, &score_x, &score_o);
      refresh();
    }

    // Quit
    if (key == 3) {
      break;
    }

    // Best Score
    if (key == 4) {
      clear();
      win = newwin(0, 0, 0, 0);
      refresh();

      win = newwin(height, width, start_y, start_x);
      score(win, score_x, score_o);
      refresh();
    }
    key = 0;
  }

  // ends the current mode
  endwin();

  return 0;
}
