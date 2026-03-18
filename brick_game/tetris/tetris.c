#include "tetris.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fsm.h"
#include "lib.h"
#include "types.h"

static game_t game;

int (*static_field_instance())[WIDTH] {
    static int field[HEIGHT][WIDTH] = {{-1}};
    if (field[0][0] == -1) {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                field[i][j] = 0;
            }
        }
    }
    return field;
}

int (*static_shadow_field())[WIDTH] {
    static int field[HEIGHT][WIDTH] = {{-1}};
    if (field[0][0] == -1) {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                field[i][j] = 0;
            }
        }
    }
    return field;
}

int **get_field() {
    int(*f)[WIDTH] = static_field_instance();
    static int *field[HEIGHT];
    for (int i = 0; i < HEIGHT; i++) {
        field[i] = f[i];
    }
    return field;
}

int **get_shadow_field() {
    int(*f)[WIDTH] = static_shadow_field();
    static int *shadow_field[HEIGHT];
    for (int i = 0; i < HEIGHT; i++) {
        shadow_field[i] = f[i];
    }
    return shadow_field;
}

game_t *static_game_instance() {
    if (!game.game_info.field) {
        game_init(&game);
    }
    return &game;
}

int game_get_state() { return (static_fsm_instance())->cur_state; }

void check_time(game_t *game) {
    // 100ms - 10 speed
    // 1000ms - 1 speed
    game->ticks++;

    int level_ticks = (100 / TIME_QUANT - 1000 / TIME_QUANT) *
                          (game->game_info.speed - 1) / 9 +
                      1000 / TIME_QUANT;
    if (game->ticks > level_ticks) {
        game->ticks = 0;
        time_event_call(game);
    }
}

void key_event_call(game_t *game, UserAction_t action) {
    game->event = key_event;
    game->action = action;
}

void time_event_call(game_t *game) {
    game->event = time_event;
    game->action = -1;
}

GameInfo_t game_update_state(game_t *game) {
    check_time(game);

    fsm_t *fsm = static_fsm_instance();
    int state = fsm->cur_state;
    if (fsm->states[state].exit) {
        fsm->states[state].exit(game);
    }

    state = fsm->states[state].action(game);

    if (fsm->states[state].enter) {
        fsm->states[state].enter(game);
    }

    fsm->cur_state = state;
    game->event = -1;
    game->action = -1;

    return game->game_info;
}

void game_init(game_t *game) {
    game->game_info.field = NULL;
    game->game_info.next = NULL;
    game->game_info.pause = 0;
    game->game_info.speed = 1;
    game->game_info.level = 1;
    game->game_info.score = 0;
    game->game_info.high_score = 0;

    game->figure_next = -1;
    game->figure = -1;
    game->pos_x = -1;
    game->pos_y = -1;
    game->side = -1;
    game->action = -1;
    game->event = -1;
    game->ticks = 0;

    set_next_figure(game);

    int score = 0;
    if (!score_from_file(&score)) {
        game->game_info.high_score = score;
    }

    game->game_info.field = get_field();
}

int is_row_all_ones(int **matrix, int row) {
    for (int j = 0; j < WIDTH; j++) {
        if (matrix[row][j] == 0) {
            return 0;
        }
    }
    return 1;
}

void remove_row(int **matrix, int row) {
    for (int i = row - 1; i >= 0; i--) {
        for (int j = 0; j < WIDTH; j++) {
            matrix[i + 1][j] = matrix[i][j];
        }
    }
    for (int j = 0; j < WIDTH; j++) {
        matrix[0][j] = 0;
    }
}

void count_score(game_t *game, int lines_removed) {
    if (lines_removed > 0) {
        int score = SINGLE_LINE;
        if (lines_removed == 2) {
            score = DOUBLE_LINES;
        } else if (lines_removed == 3) {
            score = TRIPLE_LINES;
        } else if (lines_removed == 4) {
            score = QUATRO_LINES;
        }
        game->game_info.score += score;

        int level = game->game_info.score / LEVEL_SCORS + 1;
        if (game->game_info.level < MAX_LEVEL &&
            game->game_info.level < level) {
            game->game_info.level++;
            if (game->game_info.speed < 10) {
                game->game_info.speed++;
            }
        }

        if (game->game_info.high_score < game->game_info.score) {
            game->game_info.high_score = game->game_info.score;
            store_score_to_file(game->game_info.high_score);
        }
    }
}

void remove_lines(game_t *game) {
    int **shadow = get_shadow_field();

    int rc = 0;
    for (int i = 0; i < HEIGHT; i++) {
        if (is_row_all_ones(shadow, i)) {
            remove_row(shadow, i);
            rc++;
        }
    }
    if (rc > 0) {
        count_score(game, rc);
    }
}

void rotate_figure(game_t *game) {
    if (!check_rotate(game)) {
        game->side = (game->side + 1) % 4;
    }
}

void shift_side_figure(int direction, game_t *game) {
    if ((!check_left(game) && direction == 0) ||
        (!check_right(game) && direction == 1)) {
        game->pos_x = game->pos_x + (!direction ? -1 : 1);
    }
}

void shift_down_figure(game_t *game) { game->pos_y++; }

void drop_figure(game_t *game) {
    if (!check_landing(game)) {
        shift_down_figure(game);
    }
}

bool fill_glass(game_t *game) {
    int offset = 1;
    if (game->figure_next == stick || game->figure_next == cube) offset = 2;
    return !check_free_place(game, 0, WIDTH / 2 - offset, game->figure_next,
                             game->side);
}

bool check_left(game_t *game) {
    return !check_free_place(game, game->pos_y, game->pos_x - 1, game->figure,
                             game->side);
}

bool check_right(game_t *game) {
    return !check_free_place(game, game->pos_y, game->pos_x + 1, game->figure,
                             game->side);
}

bool check_rotate(game_t *game) {
    int side = (game->side + 1) % 4;
    return !check_free_place(game, game->pos_y, game->pos_x, game->figure,
                             side);
}

bool check_landing(game_t *game) {
    return !check_free_place(game, game->pos_y + 1, game->pos_x, game->figure,
                             game->side);
}

int check_free_place(game_t *game, int pos_y, int pos_x, figure_kind kind,
                     int side) {
    (void)game;

    int **side_m = get_figure_sprite(kind, side);

    int **shadow = get_shadow_field();

    int size = 4;
    int free_place = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (free_place) {
                if (side_m[i][j]) {
                    if (pos_y + i < HEIGHT && pos_x + j < WIDTH &&
                        pos_x + j >= 0) {
                        free_place = shadow[pos_y + i][pos_x + j] == 0 ? 1 : 0;
                    } else {
                        free_place = 0;
                    }
                }
            }
        }
    }

    return free_place;
}

figure_kind pick_next_figure() { return rand() % FIGURES_SIZE; }

void set_next_figure(game_t *game) {
    figure_kind next = game->figure_next;
    if ((int)next == -1) {
        next = pick_next_figure();
    }

    game->figure_next = pick_next_figure();
    game->figure = next;

    int offset = 1;
    if (game->figure == stick || game->figure == cube) offset = 2;
    game->pos_x = WIDTH / 2 - offset;
    game->pos_y = 0;
    game->side = 0;

    game->game_info.next = get_next_sprite(game->figure_next, 0);
}

void place_figure(game_t *game) {
    int **shadow = get_shadow_field();

    int **side = get_figure_sprite(game->figure, game->side);

    int size = 4;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (side[i][j]) {
                shadow[game->pos_y + i][game->pos_x + j] = side[i][j];
            }
        }
    }

    remove_lines(game);
}

void show_figure(game_t *game) {
    int **field = get_field();

    int **side = get_figure_sprite(game->figure, game->side);

    int size = 4;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (side[i][j]) {
                field[game->pos_y + i][game->pos_x + j] = side[i][j];
            }
        }
    }
}

void update_field(game_t *game) {
    int **field = get_field();
    int **shadow = get_shadow_field();
    clean_field(field);
    copy_field(shadow, field);
    show_figure(game);
}

void clean_field(int **field) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = 0;
        }
    }
}

void copy_field(int **src, int **dst) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

void add_field(int **src, int **dst) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            dst[i][j] =
                (dst[i][j] + src[i][j]) % 2 + (dst[i][j] + src[i][j]) / 2;
        }
    }
}

int score_from_file(int *score) {
    int result = 1;

    FILE *pfile = fopen(SCORE_FILE, "r+");
    if (pfile) {
        char buffer[255];
        if (fgets(buffer, 255, pfile) != NULL) {
            int value;
            if (sscanf(buffer, "%d", &value) > 0 && value > 0) {
                *score = value;
                result = 0;
            }
        }
    } else {
        printf("n/a\n");
    }
    if (pfile) {
        fclose(pfile);
    }

    return result;
}

int store_score_to_file(int score) {
    int result = 1;

    FILE *pfile = fopen(SCORE_FILE, "w+");
    if (pfile) {
        char buffer[255];
        sprintf(buffer, "%d", score);
        fputs(buffer, pfile);
        result = 0;
    } else {
        printf("n/a\n");
    }
    if (pfile) {
        fclose(pfile);
    }

    return result;
}
