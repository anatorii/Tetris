#ifndef DEFINES_H
#define DEFINES_H

#include <ncurses.h>

#define WIDTH 10
#define HEIGHT 20
#define V_SCALE 1
#define H_SCALE 2
#define START_X 1
#define START_Y 0
#define SIZE_X(x) (x * H_SCALE)
#define SIZE_Y(y) (y * V_SCALE)
#define MVPRINTW(y, x, ...) mvprintw(START_Y + (y), START_X + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(START_Y + (y), START_X + (x), c)
#define CLEAR(y, x) mvaddch(START_Y + (y), START_X + (x), ' ')
#define TIME_QUANT 20

#define ncurses_init()        \
    {                         \
        initscr();            \
        keypad(stdscr, TRUE); \
        curs_set(FALSE);      \
        noecho();             \
        timeout(TIME_QUANT);  \
    }

#endif
