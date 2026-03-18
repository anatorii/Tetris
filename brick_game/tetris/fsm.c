#include "fsm.h"

static fsm_state_t fsm_states[STATES_SIZE] = {
    {(action_func)start_action, 0, 0, 0},
    {(action_func)idle_action, 0, (exit_func)idle_exit, (enter_func)idle_enter},
    {(action_func)moves_action, 0, 0, 0},
    {(action_func)landing_action, 0, 0, 0},
    {(action_func)next_action, 0, 0, 0},
    {(action_func)shift_action, 0, 0, 0},
    {(action_func)stop_action, 0, (exit_func)stop_exit, 0},
    {(action_func)gameover_action, 0, 0, (enter_func)gameover_enter}};

static fsm_t fsm = {-1, NULL};

fsm_t *static_fsm_instance() {
    if (!fsm.states) {
        fsm_init(&fsm);
    }
    return &fsm;
}

void fsm_init(fsm_t *fsm) {
    fsm->cur_state = START;
    fsm->states = fsm_states;
}

int start_action(game_t *game) {
    if (game->action == Start) {
        return MOVES;
    } else if (game->action == Terminate) {
        return STOP;
    }
    return START;
}

int idle_action(game_t *game) {
    if (game->event == key_event) {
        if (game->action == Pause) {
            return MOVES;
        } else if (game->action == Terminate) {
            return STOP;
        }
    }
    return IDLE;
}

int shift_action(game_t *game) {
    if (!check_landing(game)) {
        shift_down_figure(game);
        return MOVES;
    }
    return LANDING;
}

int landing_action(game_t *game) {
    place_figure(game);
    if (fill_glass(game)) {
        return GAMEOVER;
    }
    return NEXT;
}

int next_action(game_t *game) {
    game->ticks = 0;
    set_next_figure(game);
    return MOVES;
}

int moves_action(game_t *game) {
    if (game->event == time_event) {
        return SHIFT;
    } else {
        if (game->event == key_event) {
            if (game->action == Left) {
                shift_side_figure(0, game);
            } else if (game->action == Right) {
                shift_side_figure(1, game);
            } else if (game->action == Action) {
                rotate_figure(game);
            } else if (game->action == Down) {
                drop_figure(game);
            } else if (game->action == Pause) {
                return IDLE;
            } else if (game->action == Terminate) {
                return STOP;
            }
        }
    }
    return MOVES;
}

int stop_action(game_t *game) {
    (void)game;
    return STOP;
}

int gameover_action(game_t *game) {
    if (game->action == Terminate) {
        return STOP;
    }
    return GAMEOVER;
}

void idle_enter(game_t *game) { game->game_info.pause = 1; }

void idle_exit(game_t *game) { game->game_info.pause = 0; }

void gameover_enter(game_t *game) { game->game_info.speed = 0; }

void stop_exit(game_t *game) { game->game_info.pause = 2; }
