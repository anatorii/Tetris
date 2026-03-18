#ifndef LIB_H
#define LIB_H

#include <stdbool.h>

#define MYLIB_API __attribute__((visibility("default")))

typedef enum {
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Down,
    Action
} UserAction_t;

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

MYLIB_API void userInput(UserAction_t action, bool hold);
MYLIB_API GameInfo_t updateCurrentState();

#endif
