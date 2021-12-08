/********
 * Project 02 : Maze
 * COSC 208, Introduction to Computer Systems, Fall 2021
 * 
 * Part A: complete the maze_new and maze_squares_fill functions
 * Part B: implement the maze_free and maze_solve functions
 ********/

#include <stdlib.h>
#include <stdio.h>
#include "maze.h"

//helper function prototype 
void maze_squares_fill(struct maze*, FILE *);

/*
 * Return a newly-heap-allocated maze struct filled with data from the passed 
 * file.
 * 
 * The provided starter code only echos the content of the file at the console
 * using the partially implemented helper function called maze_squares_fill.
 * Make sure to allocate the struct maze and fill its content according to the * content of the file.
 */
//allocates memory for the maze data and the 2d array representing it. Also sets the number of rows and columns based on data from the file. 
struct maze *maze_new(const char *filename) { 
  int rows, cols;
  FILE *in_file;
  in_file = fopen(filename, "r");
  struct maze *maze1 = malloc(sizeof(struct maze));//allocat space for maze struct
  if (in_file == NULL) {
    printf("Can't open file for reading.\n");
  }
  else {
    fscanf(in_file, "%d", &rows);
    fscanf(in_file, "%d", &cols);
    printf("rows %d and columns %d \n", rows, cols);
    maze1->rows = rows;
    maze1->cols = cols;
    struct square *data = malloc(sizeof(struct square) * rows * cols);//allocate space for every single square in the grid. 
    maze1->data = data;
    maze_squares_fill(maze1, in_file);
    fclose(in_file);
  }

  return maze1;
}

/*
 * Populate the struct maze with grid data from the file.
 *
 * The provided starter only echos the content of the file maze1 at the 
 * console. 
 * 
 * Notice:
 *  - bd is NULL when called by incomplete maze_new function <- change once you 
 *    allocate the struct
 *  - Hard-coded nested loop to read the cols of each row: 3x3 which works with
 *    maze1
 *  - Code should use and update the passed struct maze 
 */
//iterates through entire pseudo 2d array made from normal array and adds appropriate square structs based on data read from file. Sets start and exit fields in maze struct when they are found.
void maze_squares_fill(struct maze* bd, FILE *fptr) {
  int num = 0;
  //double for loop iterates through entire 2d array. 
  for(int i = 0; i < bd->rows; i++) {
	  for(int j = 0; j < bd->cols; j++){
   	  fscanf(fptr, "%d", &num);
      //create a new square struct and set values based on file data.
      struct square current;
      current.row = i;
      current.col = j;
      current.state = num;
      current.standing = TOEXPLORE;
      current.parent = NULL;
      bd->data[(bd->cols)*i + j] = current;
      //if current square is a start or exit, set the appropriate field in maze struct. 
      if(num == START){
        bd->start = &bd->data[(bd->cols)*i + j];
      } else if(num == EXIT){
        bd->exit = &bd->data[(bd->cols)*i + j];
      }
    }
  }
}
//Frees all data on the heap. Only two mallocs were made in maze_new
void maze_free(struct maze *bd) {
  free(bd->data);
  free(bd);
}

/*
 * Print the content of a struct square (for debugging purposes).
 */
void square_print(const struct square *sq) {
  if (sq != NULL) {
    printf("Square at %d %d ", sq->row, sq->col);
    printf("is of state %d, and has standing '%c'\n", sq->state, sq->standing);
  } else {
    printf("square_print: the square provided is null \n"); 
  }
}

/*
 * Print the data array of the struct maze passed at the console. 
 */
void maze_print(const struct maze* bd) {
  if (bd != NULL) {
    int i, j;
    // print row of column numbers 
    printf("  ");
    for(i = 0; i < bd->cols; i++)
        printf("%d ", i % 10);   // digit only
    printf("\n");

    // print maze starting w/ row numbers
    for(i = 0; i < bd->rows; i++) {
            printf("%d ", i % 10);
        for(j = 0; j < bd->cols; j++) {
            struct square* sq = &bd->data[i * bd->cols + j]; 

            if (sq == bd->start) {
                printf("S ");
            } else if (sq == bd->exit) {
               printf("E ");
            }
            else if (sq->state == WALL) {
                printf("# ");
            } 
            else if (sq->standing == ONPATH) {
                printf("* ");
            } else {
                printf("_ ");
            } 
        }
        printf("\n");
    } 
  } else {
    printf("maze not allocated\n");
  }
}

/////////////  Part B SOLVER ///////////////

/*
 * Find path from entry to exit using a depth-first-search algorithm
 *  - if a path is found, reconstitute the path by updating standing of the 
 *    squares on the path and return the length of the path
 *  - otherwise return -1
 */ 

//helper function for adding an adjacent square to the stack
int explore_direction(struct maze *bd, struct stack *stk, struct square *temp, int row, int col) { // push squares adjacent to the parent to the stack
    if ((row >= 0 && row < bd->rows) && (col >= 0 && col < bd->cols)) { // ensure program does not go out of bounds
      struct square *sq = bd->data+(bd->cols*(row)+col);
      if ((sq->state == EMPTY||sq->state == EXIT) && sq->standing == TOEXPLORE ){ // only push valid squares to the stack
        stack_push(stk, sq);
        stk->top->val->parent = temp; // set last-pushed square's parent
      }
      if(sq->state == EXIT)
        return 1; // return true if a square is identified as the special exit, ensure the exit square takes priority
    }
    return 0;
}

int maze_solve(struct maze *bd) {
  struct stack *stk = stack_new(); //create exploration stack 
  int pathlen = 0;
  stack_push(stk, bd->start);//start path at start square

  while(stk->top != NULL) {//if the stack is empty that means every possible reachable square has been explored without finding the exit. 
    struct square *temp = stack_pop(stk);//pop the most recently seen square and save it. 
    int col = temp->col;
    int row = temp->row;

    if (temp->state == EXIT) { //if it's the exit trace back and mark the path back to the start with parent field in struct square, incrementing the path length as you go.
      while(temp != NULL){
        temp->standing = ONPATH;
        temp = temp->parent;
        pathlen++;
      }
      printf("Path to exit found!\n");
      stack_free(stk);
      return pathlen;
    }
    //if the exit is found when exploring any of the adjacent squares, continue to the next iteration to ensure the exit is on top of the stack to ensure the next iteration of the loop finds it immediately and enters the traceback block. 
    if (explore_direction(bd, stk, temp, row, (col - 1)))
      continue;
    if(explore_direction(bd, stk, temp, row, (col + 1)))
      continue;
    if(explore_direction(bd, stk, temp, (row - 1), col))
      continue;
    if(explore_direction(bd, stk, temp, (row + 1), col))
      continue;
    temp->standing = EXPLORED;  //mark current square as explored because all of its neighbors have been added to the stack. 
  }
  stack_free(stk);
  return -1;
}   