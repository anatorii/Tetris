#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../brick_game/tetris/figures.h"
#include "../brick_game/tetris/fsm.h"
#include "../brick_game/tetris/lib.h"
#include "../brick_game/tetris/tetris.h"

void test_figures() {
    for (int f = 0; f < FIGURES_SIZE; f++) {
        for (int s = 0; s < 4; s++) {
            int **side = get_figure_sprite(f, s);
            int **next = get_next_sprite(f, s);
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    printf("%d", side[i][j]);
                }
                printf("|");
                for (int j = 0; j < 4; j++) {
                    printf("%d", next[i][j]);
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    }
}

void test_fsm() {
    fsm_t *fsm = static_fsm_instance();

    game_t *game = static_game_instance();

    for (int state = 0; state < STATES_SIZE; state++) {
        if (fsm->states[state].exit) {
            fsm->states[state].exit(game);
        }
    }

    for (int state = 0; state < STATES_SIZE; state++) {
        if (fsm->states[state].action) {
            fsm->states[state].action(game);
        }
    }

    for (int state = 0; state < STATES_SIZE; state++) {
        if (fsm->states[state].enter) {
            fsm->states[state].enter(game);
        }
    }

    for (int state = 0; state < STATES_SIZE; state++) {
        if (fsm->states[state].guard) {
            fsm->states[state].guard(game);
        }
    }
}

void test_moves_action() {
    game_t *game = static_game_instance();

    game->event = key_event;
    for (int i = 0; i < 7; i++) {
        game->action = i;
        moves_action(game);
    }
}

void test_idle_action() {
    game_t *game = static_game_instance();

    game->event = key_event;
    for (int i = 0; i < 7; i++) {
        game->action = i;
        idle_action(game);
    }
}

void test_shift_action() {
    game_t *game = static_game_instance();

    shift_action(game);
}

void test_store_score_to_file() {
    game_t *game = static_game_instance();
    store_score_to_file(game->game_info.high_score);
}

void test_show_figure() {
    game_t *game = static_game_instance();
    show_figure(game);
}

void test_update_field() {
    game_t *game = static_game_instance();
    update_field(game);
}

void test_clean_field() {
    int **field = get_field();
    clean_field(field);
}

void test_copy_field() {
    int **field = get_field();
    int **shadow = get_shadow_field();
    copy_field(shadow, field);
}

void test_add_field() {
    int **field = get_field();
    int **shadow = get_shadow_field();
    add_field(shadow, field);
}

void test_count_score() {
    game_t *game = static_game_instance();
    game->game_info.high_score = 0;
    game->game_info.score = 100;
    count_score(game, 1);
    count_score(game, 2);
    count_score(game, 3);
    count_score(game, 4);
}

void test_game_update_state() {
    game_t *game = static_game_instance();
    game_update_state(game);
}

void test_userInput() { userInput(Start, false); }

void test_updateCurrentState() {
    GameInfo_t info = updateCurrentState();
    printf("score:%d\n", info.score);
}

START_TEST(test_1) { test_figures(); }
END_TEST

START_TEST(test_2) { test_fsm(); }
END_TEST

START_TEST(test_3) { test_moves_action(); }
END_TEST

START_TEST(test_4) { test_idle_action(); }
END_TEST

START_TEST(test_shift_action_1) { test_shift_action(); }
END_TEST

START_TEST(test_5) { test_store_score_to_file(); }
END_TEST

START_TEST(test_6) { test_add_field(); }
END_TEST

START_TEST(test_7) { test_clean_field(); }
END_TEST

START_TEST(test_8) { test_copy_field(); }
END_TEST

START_TEST(test_9) { test_show_figure(); }
END_TEST

START_TEST(test_10) { test_update_field(); }
END_TEST

START_TEST(test_11) { test_count_score(); }
END_TEST

START_TEST(test_12) { test_game_update_state(); }
END_TEST

START_TEST(test_13) { test_userInput(); }
END_TEST

START_TEST(test_14) { test_updateCurrentState(); }
END_TEST

Suite *tetris_suite(void) {
    Suite *s = suite_create("tetris_tests");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_1);
    tcase_add_test(tc_core, test_2);
    tcase_add_test(tc_core, test_3);
    tcase_add_test(tc_core, test_4);
    tcase_add_test(tc_core, test_shift_action_1);
    tcase_add_test(tc_core, test_5);
    tcase_add_test(tc_core, test_6);
    tcase_add_test(tc_core, test_7);
    tcase_add_test(tc_core, test_8);
    tcase_add_test(tc_core, test_9);
    tcase_add_test(tc_core, test_10);
    tcase_add_test(tc_core, test_11);
    tcase_add_test(tc_core, test_12);
    tcase_add_test(tc_core, test_13);
    tcase_add_test(tc_core, test_14);
    suite_add_tcase(s, tc_core);
    return s;
}

int test_tetris(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = tetris_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
