#ifndef __MAZE_H__
#define __MAZE_H__

#define EMPTY 0
#define WALL 1
#define START 2
#define EXIT 3

#define TOEXPLORE 'o'
#define EXPLORED '.'
#define ONPATH 'x'


#define WEST 0
#define NORTH 1
#define EAST 2
#define SOUTH 3

#define NOT_FOUND -1

#include "stack.h"

struct square {
    int state;  // 0, 1, 2, 3 in file
               // in file 0: ' ' empty space
               //         1: '#' wall
               //         2: 'S' start
               //         3: 'E' exit    USE the constants #define above
    char standing; // 'o', '.', 'x'
    int row;
    int col;
    struct square *parent;
};

struct maze{
    int rows;
    int cols;
    struct square *start;
    struct square *exit;
    struct square *data;   
};

// provided and complete
void square_print(const struct square*);
void maze_print(const struct maze*);

// TODO Part A: complete in maze.c
struct maze* maze_new(const char*);
void maze_free(struct maze*);

// TODO Part B: complete in maze.c
int maze_solve(struct maze*);

#endif // __MAZE_H__