#include "render.h"

#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void show_border() {
    mvaddch(START_Y, START_X, ACS_ULCORNER);
    mvaddch(START_Y, START_X + SIZE_X(WIDTH) + 1, ACS_URCORNER);
    mvaddch(START_Y + SIZE_Y(HEIGHT) + 1, START_X + SIZE_X(WIDTH) + 1,
            ACS_LRCORNER);
    mvaddch(START_Y + SIZE_Y(HEIGHT) + 1, START_X, ACS_LLCORNER);
    mvhline(START_Y, START_X + 1, ACS_HLINE, SIZE_X(WIDTH));
    mvvline(START_Y + 1, START_X + SIZE_X(WIDTH) + 1, ACS_VLINE,
            SIZE_Y(HEIGHT));
    mvhline(START_Y + SIZE_Y(HEIGHT) + 1, START_X + 1, ACS_HLINE,
            SIZE_X(WIDTH));
    mvvline(START_Y + 1, START_X, ACS_VLINE, SIZE_Y(HEIGHT));
}

void show_glass(GameInfo_t *game) {
    if (game->field) {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                int ch = ACS_BULLET;
                if (game->field[i][j] != 0)
                    attron(COLOR_PAIR(game->field[i][j]));
                MVADDCH(i + 1, j * 2 + 1, ch);
                MVADDCH(i + 1, j * 2 + 1 + 1, ch);
                if (game->field[i][j] != 0)
                    attroff(COLOR_PAIR(game->field[i][j]));
            }
        }
    }
}

void show_next_figure(GameInfo_t *game, int pos_y, int pos_x) {
    if (game->next) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                CLEAR(i + pos_y, j * 2 + pos_x);
                CLEAR(i + pos_y, j * 2 + 1 + pos_x);
                if (game->next[i][j] != 0) {
                    attron(COLOR_PAIR(game->next[i][j]));
                    MVADDCH(i + pos_y, j * 2 + pos_x, 32);
                    MVADDCH(i + pos_y, j * 2 + 1 + pos_x, 32);
                    attroff(COLOR_PAIR(game->next[i][j]));
                }
            }
        }
    }
}

void show_score_info(GameInfo_t *game) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    MVPRINTW(0, SIZE_X(WIDTH) + 4, "%02d:%02d:%02d", local->tm_hour,
             local->tm_min, local->tm_sec);

    MVPRINTW(2, SIZE_X(WIDTH) + 5, "LEVEL:");
    MVPRINTW(3, SIZE_X(WIDTH) + 5, "%4d", game->level);
    MVPRINTW(5, SIZE_X(WIDTH) + 5, "SCORE:");
    MVPRINTW(6, SIZE_X(WIDTH) + 5, "%4d", game->score);
    MVPRINTW(8, SIZE_X(WIDTH) + 5, "SPEED:");
    MVPRINTW(9, SIZE_X(WIDTH) + 5, "%4d", game->speed);
    MVPRINTW(11, SIZE_X(WIDTH) + 5, "HIGH SCORE:");
    MVPRINTW(12, SIZE_X(WIDTH) + 5, "%4d", game->high_score);
    MVPRINTW(14, SIZE_X(WIDTH) + 5, "NEXT:");

    MVPRINTW(20, SIZE_X(WIDTH) + 5, "         ");
    if (game->pause == 1) {
        MVPRINTW(20, SIZE_X(WIDTH) + 5, "PAUSE    ");
    } else if (game->speed == 0) {
        MVPRINTW(20, SIZE_X(WIDTH) + 5, "GAME OVER");
    }
}

void show_field(GameInfo_t *game) {
    show_border();
    show_glass(game);
    show_score_info(game);
    show_next_figure(game, 16, SIZE_X(WIDTH) + 5);
    refresh();
}
