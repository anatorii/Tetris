#include "game.h"

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(0));
    ncurses_init();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(4, COLOR_WHITE, COLOR_GREEN);
    init_pair(5, COLOR_WHITE, COLOR_CYAN);
    init_pair(6, COLOR_WHITE, COLOR_RED);
    init_pair(7, COLOR_WHITE, COLOR_YELLOW);

    int act = -1;
    GameInfo_t game_info;
    while (game_info.pause != 2) {
        game_info = updateCurrentState();
        act = input_to_action(getch());
        if (act > -1) userInput((UserAction_t)act, 0);
        show_field(&game_info);
    }

    getch();

    endwin();

    return 0;
}
