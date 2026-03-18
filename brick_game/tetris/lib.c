#include "lib.h"

#include <ncurses.h>
#include <stdbool.h>
#include <time.h>

#include "fsm.h"
#include "tetris.h"

MYLIB_API void userInput(UserAction_t action, bool hold) {
    (void)hold;
    game_t *game = static_game_instance();
    key_event_call(game, action);
}

MYLIB_API GameInfo_t updateCurrentState() {
    game_t *game = static_game_instance();
    GameInfo_t info = game_update_state(game);
    update_field(game);
    return info;
}
