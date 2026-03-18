#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdio.h>

#include "figures.h"
#include "lib.h"

typedef struct {
    GameInfo_t game_info;
    figure_kind figure;
    figure_kind figure_next;
    int side;
    int pos_x;
    int pos_y;
    int event;
    int action;
    int ticks;
} game_t;

game_t *static_game_instance();
int **get_field();
int **get_shadow_field();
int game_get_state();
void check_time(game_t *game);
void key_event_call(game_t *game, UserAction_t action);
void time_event_call(game_t *game);
GameInfo_t game_update_state(game_t *game);
void game_init(game_t *game);
void count_score(game_t *game, int lines_removed);
void remove_lines(game_t *game);
void rotate_figure(game_t *game);
void shift_side_figure(int direction, game_t *game);
void shift_down_figure(game_t *game);
void drop_figure(game_t *game);
bool fill_glass(game_t *game);
bool check_left(game_t *game);
bool check_right(game_t *game);
bool check_rotate(game_t *game);
bool check_landing(game_t *game);
int check_free_place(game_t *game, int pos_y, int pos_x, figure_kind kind,
                     int side);
figure_kind pick_next_figure();
void set_next_figure(game_t *game);
void place_figure(game_t *game);
int score_from_file(int *score);
int store_score_to_file(int score);

void show_figure(game_t *game);
void update_field(game_t *game);
void clean_field(int **field);
void copy_field(int **src, int **dst);
void add_field(int **src, int **dst);

#endif
