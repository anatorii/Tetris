#include "utils.h"

#include <string.h>

#include "../../brick_game/tetris/lib.h"
#include "defines.h"
#include "render.h"

int input_to_action(int input) {
    int action = -1;

    if (input == KEY_LEFT)
        action = Left;
    else if (input == KEY_RIGHT)
        action = Right;
    else if (input == KEY_DOWN)
        action = Down;
    else if (input == 32)
        action = Action;
    else if (input == 'q' || input == 'Q')
        action = Terminate;
    else if (input == '\n')
        action = Start;
    else if (input == 'p' || input == 'P')
        action = Pause;
    else
        action = -1;

    return action;
}
