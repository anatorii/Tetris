#ifndef RENDER_H
#define RENDER_H

#include "../../brick_game/tetris/lib.h"
#include "defines.h"
#include "utils.h"

void show_border();
void show_glass(GameInfo_t *game);
void show_next_figure(GameInfo_t *game, int pos_y, int pos_x);
void show_score_info(GameInfo_t *game);
void show_field(GameInfo_t *game);

#endif
