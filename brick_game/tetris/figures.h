#ifndef FIGURES_H
#define FIGURES_H

#include "types.h"

typedef enum {
    stick,
    poker,
    poker_mirror,
    cube,
    horse,
    horse_mirror,
    podium
} figure_kind;

int (*static_figure_instance())[4][4][4];
void fill_figures(int figures[FIGURES_SIZE][4][4][4]);
void fill_figure(char *data, int (*figure)[4][4], int kind);
int **get_figure_sprite(figure_kind kind, int pos);
int **get_next_sprite(figure_kind kind, int pos);

#endif
