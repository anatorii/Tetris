#include "figures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int (*static_figure_instance())[4][4][4] {
    static int figures[FIGURES_SIZE][4][4][4] = {{{{-1}}}};
    if (figures[0][0][0][0] == -1) {
        fill_figures(figures);
    }
    return figures;
}

void fill_figures(int figures[FIGURES_SIZE][4][4][4]) {
    fill_figure(
        "\
            0000\
            1111\
            0000\
            0000\
            0010\
            0010\
            0010\
            0010\
            0000\
            0000\
            1111\
            0000\
            0100\
            0100\
            0100\
            0100\
        ",
        (int(*)[4][4]) & figures[stick], stick);

    fill_figure(
        "\
            0110\
            0110\
            0000\
            0000\
            0110\
            0110\
            0000\
            0000\
            0110\
            0110\
            0000\
            0000\
            0110\
            0110\
            0000\
            0000\
        ",
        (int(*)[4][4]) & figures[cube], cube);

    fill_figure(
        "\
            0100\
            1110\
            0000\
            0000\
            0100\
            0110\
            0100\
            0000\
            0000\
            1110\
            0100\
            0000\
            0100\
            1100\
            0100\
            0000\
        ",
        (int(*)[4][4]) & figures[podium], podium);

    fill_figure(
        "\
            1000\
            1110\
            0000\
            0000\
            0110\
            0100\
            0100\
            0000\
            0000\
            1110\
            0010\
            0000\
            0100\
            0100\
            1100\
            0000\
        ",
        (int(*)[4][4]) & figures[poker], poker);

    fill_figure(
        "\
            0010\
            1110\
            0000\
            0000\
            0100\
            0100\
            0110\
            0000\
            0000\
            1110\
            1000\
            0000\
            1100\
            0100\
            0100\
            0000\
        ",
        (int(*)[4][4]) & figures[poker_mirror], poker_mirror);

    fill_figure(
        "\
            0110\
            1100\
            0000\
            0000\
            0100\
            0110\
            0010\
            0000\
            0000\
            0110\
            1100\
            0000\
            1000\
            1100\
            0100\
            0000\
        ",
        (int(*)[4][4]) & figures[horse], horse);

    fill_figure(
        "\
            1100\
            0110\
            0000\
            0000\
            0010\
            0110\
            0100\
            0000\
            0000\
            1100\
            0110\
            0000\
            0100\
            1100\
            1000\
            0000\
        ",
        (int(*)[4][4]) & figures[horse_mirror], horse_mirror);
}

void fill_figure(char *data, int (*figure)[4][4], int kind) {
    char *cell = data;
    int index = 0;
    for (int r = 0; r < 4; r++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                index = strcspn(cell, "01");
                cell += index;
                figure[r][i][j] = *cell != '0' ? kind + 1 : 0;
                cell++;
            }
        }
    }
}

int **get_figure_sprite(figure_kind kind, int pos) {
    int(*figures)[4][4][4] = static_figure_instance();

    if (kind < 0 || kind >= FIGURES_SIZE || pos < 0 || pos >= 4) {
        return NULL;
    }

    static int *sprite[4];
    for (int i = 0; i < 4; i++) {
        sprite[i] = figures[kind][pos][i];
    }
    return sprite;
}

int **get_next_sprite(figure_kind kind, int pos) {
    int(*figures)[4][4][4] = static_figure_instance();

    if (kind < 0 || kind >= FIGURES_SIZE || pos < 0 || pos >= 4) {
        return NULL;
    }

    static int *next[4];
    for (int i = 0; i < 4; i++) {
        next[i] = figures[kind][pos][i];
    }
    return next;
}
