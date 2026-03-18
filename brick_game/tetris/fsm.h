
#ifndef FSM_H
#define FSM_H

#include "tetris.h"

#define STATES_SIZE 8
#define EVENTS_SIZE 5

typedef int (*action_func)(void *);
typedef int (*guard_func)(void *);
typedef void (*exit_func)(void *);
typedef void (*enter_func)(void *);

typedef enum {
    START = 0,
    IDLE = 1,
    MOVES = 2,
    LANDING = 3,
    NEXT = 4,
    SHIFT = 5,
    STOP = 6,
    GAMEOVER = 7
} game_state_t;

typedef enum {
    key_event = 0,
    time_event = 1,
    score_event = 2,
    glass_event = 3,
    onground_event = 4
} event_kind_t;

typedef struct {
    action_func action;
    guard_func guard;
    exit_func exit;
    enter_func enter;
} fsm_state_t;

typedef struct {
    short cur_state;
    fsm_state_t *states;
} fsm_t;

fsm_t *static_fsm_instance();
void fsm_init(fsm_t *fsm);

int start_action(game_t *game);
int idle_action(game_t *game);
int shift_action(game_t *game);
int landing_action(game_t *game);
int next_action(game_t *game);
int moves_action(game_t *game);
int stop_action(game_t *game);
int gameover_action(game_t *game);

void idle_enter(game_t *game);
void idle_exit(game_t *game);
void gameover_enter(game_t *game);
void stop_exit(game_t *game);

#endif
