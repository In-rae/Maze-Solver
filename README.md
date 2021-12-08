This is a reupload. Additional credit to my partner Malcolm Bell, credit to Colgate University for starter code.
# Maze-Solver


A maze is a 2D grid defined in a file, such as those found in the `maze` folder. The file format is as follows:
* the first line contains two integers: number of rows (R) followed by number of columns (C) for the 2D grid, 
* each R line thereafter provides C integers (`[0-3]`).  

For example, `maze/maze2` contains the following data
```
7 13
0 0 0 0 0 0 1 0 0 0 0 0 0
1 1 0 1 1 1 1 1 1 1 0 1 0
0 1 0 0 0 0 0 0 0 0 0 1 0
0 1 0 1 1 0 0 1 1 1 0 1 0
0 0 0 1 0 0 0 0 0 1 0 1 0
0 1 1 1 1 1 0 1 0 1 0 1 0
0 0 0 0 2 0 0 1 0 0 1 3 0
```

The `[0-3]` values of the maze encode four *states*:
```
0 - empty 
1 - wall
2 - start
3 - exit
```
<!--Each square is empty, a wall, or one of two special squares used to represent the entry and the exit points. -->

The `maze_print` function in `maze.c` displays a maze in the following way, emphazising the visual difference between squares that are empty (`_`) and wall (`#`). 

```
  0 1 2 3 4 5 6 7 8 9 0 1 2 
0 _ _ _ _ _ _ # _ _ _ _ _ _ 
1 # # _ # # # # # # # _ # _ 
2 _ # _ _ _ _ _ _ _ _ _ # _ 
3 _ # _ # # _ _ # # # _ # _ 
4 _ _ _ # _ _ _ _ _ # _ # _ 
5 _ # # # # # _ # _ # _ # _ 
6 _ _ _ _ S _ _ # _ _ # E _ 
```
Running the main program with a valid maze file as input will return a printout of the maze with the solution highlighted with * , if one exists. The program is implemented with a depth-first search using a stack that keeps track of parent tiles so that it can reproduce the solution path one the search finds the exit. 


