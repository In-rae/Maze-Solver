# Project 02: Heapify maze solver

## Overview
In this project, you'll determine a path out of a maze using a stack. The `maze` and the `stack` data structure will be stored on the heap!

### Learning objectives
After completing this project, you should be able to:
* Implement linked data structures (e.g., a stack) using dynamically allocate heap memory in C
* Implement 2D arrays using dynamically allocated heap memory in C
* Write functions in C that take pointers as parameters and/or return pointers
* Use a `stack` to backtrack and solve a maze
* Use valgrind to diagnose memory errors/leaks in C programs 

### Logistics
The project is divided into two parts. Part A must be completed by **Thursday, October 21 at  11pm**. Part B, must be completed by **Thursday, November 4 at 11pm**.

You will receive feedback and a grade on Part A by **Friday, October 29**. You should review this feedback to learn from it and correct memory errors/leaks. You may earn back up to half of the points you lost. Note that you will not have an opportunity to revise your code or earn back points for Part B, so you should use the feedback you receive on Part A to avoid making similar mistakes in Part B.

### Important tips
* Read the entirety of the project description before you write any code.
* Work on the project over multiple sessions. Do not try to complete the project in a single session on the day (before) each part of the project is due. This will give you time to think about how to solve problems, allow you to ask questions, and result in better outcomes.
* Ensure you follow good program design, coding, testing, and debugging practices (details below).

## Getting started 
Clone your repository on the COSC 208 servers.

The code you need to write for this project will be added to the `maze.c` and `stack.c` files included in your repository. When you want to compile your code, run the command
```bash
$ make
```

You can (partially) test your code for by running the two included tester programs `test_maze`, `test_stack`, and `test_solve`.

In part A, you will: 
1. Complete the `maze_new`, `maze_squares_fill`, and `maze_free` functions in `maze.c` to store maze data in a `struct maze` allocated in heap space.
2. Implement the `stack_new`, `stack_push`, `stack_pop`, and `stack_push` functions in `stack.c` to manage a `struct stack` in heap space.

In part B, you will: implement `maze_solve` in `maze.c` to use a stack of squares which maintains the current path being explored and solve the maze according to the a depth-first-search algorithm.

## Part A: Data structures

### maze representation

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
<!--The following picture is its corresponding graphical representation 

Indeed, each square is either empty or a wall, or one of two special squares to represent the entry and the exit of the maze. The green square is the entry, the red the exit to reach, the black ones are walls while the blue ones are empty. The goal, you will implement in Part 2, is to determine a path of blue squares connecting the green entry to the red exit.
-->

<!--In `maze.h`, there are 2 `struct` definitions to represent this data.
-->
In Part B, you will implement an algorithm that determines a path among the empty squares (`_`), which connects the start (`S`) to the exit (`E`), as shown below with `*`. 
```
  0 1 2 3 4 5 6 7 8 9 0 1 2 
0 _ _ _ _ _ _ # _ _ _ * * * 
1 # # _ # # # # # # # * # * 
2 _ # _ _ _ _ * * * * * # * 
3 _ # _ # # _ * # # # _ # * 
4 _ _ _ # _ _ * _ _ # _ # * 
5 _ # # # # # * # _ # _ # * 
6 _ _ _ _ S * * # _ _ # E * 
```

### maze structs 

The maze representation is made of two structures in C: `struct square` and `struct maze`, which are defined in `maze.h`. 

A `struct square` consists of 5 fields: 
1. its row in the maze (using zero-based indexing)
2. its column in the maze (using zero-based indexing)
3. a state - an integer (as defined in the maze file and explained above; does not change after initialization)
4. a standing with respect to the solver - a character (initialized as `'o'` meaning _to be explored_, and updated in Part B to `'.'` when it has been _explored_ and `'x'` when it is _on the path_ connecting the start to the exit of the maze)
5. a parent - a reference to the square before it on the connecting path (initialized to `NULL`, and updated by the path finding algorithm in Part B)

A `struct maze` has also 5 fields
1. the total number of rows
2. the total number of columns
3. a reference to the starting `struct square`
4. a reference to the exit `struct square`
5. a dynamically allocated array of `struct squares` which define the maze

### maze functions 

You need to complete three functions in `maze.c` for Part A of the project: 

1. `maze_new` takes a filename (e.g., `"maze/maze1"`) and returns the corresponding
heap allocated `struct maze*` representation. Its `struct square*` field should be a dynamically allocated 2D array (using method 1 of _Dive Into Systems_ [Section 2.5.2](https://diveintosystems.org/book/C2-C_depth/arrays.html#_method_1_memory_efficient_allocation)).
2. `maze_fill_squares` is a helper function used by `maze_new` to read the integer grid of the provided file. For now, those values are echoed at the console. You need to store them to initialize the `struct square` of the maze with the appropriate row, column, state, standing, and parent values, as defined above.
3. `maze_free` deallocates the heap memory for the `struct maze*` allocated by `maze_new`.

A `maze_print` function is provided in `maze.c`; use it to debug and display the maze.

### stack structs 
Similar to lab 6, your stack uses two structs, which are defined in `stack.h`. The `struct stack` contains a `struct node` field, which is a self-referential structure. 

A `struct stack` holds a pointer to the top of the `stack`—i.e., the last `struct node` pushed on the stack.  If the stack is empty, then the top of the stack should be `NULL`. A `struct node` holds a pointer to a square and a pointer to the next `struct node` in the stack. The bottom node on the stack  should not point to anything—i.e., its `next` field should be `NULL`.

For example, the following memory diagram depicts a stack for which `square[0,0]` was first pushed, followed by `square[1,1]`. 

```
                                                   ┌-------------┐               ┌-------------┐
                                               ┌~~>| square[1,1] |           ┌~~>| square[0,0] |
                                               |   └-------------┘           |   └-------------┘
┌--------------------┐  ┌----------------------|-┐    ┌----------------------|-┐
| struct node *top ~~~~>| struct square *val ~~┘ | ╭~>| struct square *val ~~┘ |
|                    |  | struct node *next ~~~~~~~╯  | struct node *next=NULL |
└--------------------┘  └------------------------┘    └------------------------┘
  struct stack            struct node                   struct node
```

### `stack` functions

You need to complete four functions in `stack.c` for Part A of the project:

1. `stack_new` allocates and initializes a `struct stack` on the heap. The fields in the `struct stack` should be initialized to represent an empty stack.
2. `stack_push` pushes a heap allocated `struct node` to the top of the passed stack. The `val` field of this new node is made to point to the integer provided by the user in heap space.
3. `stack_pop` removes the `struct node` at the top of the stack and returns the heap address of the integer previously residing at the top of the stack.   
4. `stack_free` frees all memory consumed by a stack, including each `struct node`, their integers and the `struct stack`.

A `stack_print` function is provided in `stack.c`; use it debug and display the stack.

### Testing and debugging  

We have provided two test programs—`test_maze` and `test_stack`—to help you test your implementations of the above maze and stack functions, respectively. These test programs cover the basic behavior, but they are not comprehensive. You should add additional testing code in each of the test programs to cover more cases and run `valgrind` to ensure you have no memory errors or leaks.

If the program compiles without errors or warnings but still doesn't work as expected, then run your code with valgrind. To help diagnose memory leaks, include the command line options `--track-origins=yes` and `--leak-check=full`. For example:
```bash
$ valgrind --track-origins=yes --leak-check=full ./test_maze
```
Valgrind **should not report any leaks or memory errors** when you run `test_maze` or `test_stack` with it.

## Part B: Solving the maze

You must implement a path finder algorithm in `maze.c` by completing `maze_solve(struct maze*)`.

### Path algorithm
Using a stack of squares which maintains the current path being explored, you will solve the maze according to the following depth-first-search algorithm.

* Create an empty stack, to hold references to each square as they are explored. 
* Start by pushing the special entry square of the maze on this stack.

* Thereafter, repeat the following steps:
  1. Is the stack empty? If so, the exit is unreachable: return -1
  2. Otherwise, pop the square to explore from the top of the stack.
  3. Is the square the special exit one? If so, the exit is reached: print `"Path to exit found!\n"` and terminate this search algorithm. 
  4. Otherwise, it is a reachable empty square that hasn't been explored yet. So, explore it as follows:
     * compute all the adjacent locations that are inside the maze and are not walls and
     * push them as long as they have not previously been explored to the stack for later exploration.
    You should explore squares in a consistent order: your path should go first south, then north, followed by east and last west.
  5. Update this square' standing as explored, i.e. `'.'` so to not explore it again. Note that a square is *explored* once its neighbors have been pushed on the stack. The neighbors themselves are not *explored* until they are popped from the stack and checked for their neighbors.

Once you terminate this search algorithm and found a path, you reconstruct it going backward: each square on the path should have `x` for their standing, to be printed as `maze_print`. To do so, each square needs to maintain a reference to its parent as they are processed on the stack (their parent is the square that pushed them). Finally call `maze_print` to check your path found.

**Do not put all of your maze solving code in the `maze_solve` function.** Add helper functions (as necessary) to `maze.c`.

### Testing and debugging  

We have provided sample mazes (in the `maze` directory) and a program (`test_solve`) that calls your maze functions to help you test your solver. The provided mazes cover the basic behavior, but they are not comprehensive. You should add additional mazes to cover more cases and run `valgrind` to ensure you have no memory errors or leaks.

If the program compiles without errors or warnings but still doesn't work as expected, then run your code with valgrind. To help diagnose memory leaks, include the command line options `--track-origins=yes` and `--leak-check=full`. For example:
```bash
$ valgrind --track-origins=yes --leak-check=full ./test_solve
```

### Sample output
Here are the expected solutions to the provided mazes:
```C
Maze 1
  0 1 2 
0 S _ _ 
1 * _ _ 
2 * * E 


Maze 2
  0 1 2 3 4 5 6 7 8 9 0 1 2 
0 _ _ _ _ _ _ # _ _ _ * * * 
1 # # _ # # # # # # # * # * 
2 _ # _ _ _ _ * * * * * # * 
3 _ # _ # # _ * # # # _ # * 
4 _ _ _ # _ _ * _ _ # _ # * 
5 _ # # # # # * # _ # _ # * 
6 _ _ _ _ S * * # _ _ # E * 


Maze 3
No path to exit found!


Maze 4
  0 1 2 3 4 5 6 7 8 9 
0 * * * * * * * * * E 
1 * # # # # # # # # # 
2 * # * * * * * * * # 
3 * # * # # # # # * # 
4 * # * # * * * # * # 
5 * # * # S # * # * # 
6 * # * # # # * # * # 
7 * # * * * * * # * # 
8 * # # # # # # # * # 
9 * * * * * * * * * # 


Maze 5
  0 1 2 3 4 5 6 7 8 9 
0 _ _ _ _ _ * * * * E 
1 _ _ _ _ _ * * * _ _ 
2 _ _ _ _ _ * * * _ _ 
3 _ _ _ _ _ * * * _ _ 
4 _ _ _ _ _ * * * _ _ 
5 _ _ _ _ S * * * _ _ 
6 _ _ _ _ * * * * _ _ 
7 _ _ _ _ * * * * _ _ 
8 _ _ _ _ * * * * _ _ 
9 _ _ _ _ * * * * _ _
```

## Program design

You **must follow good program design and coding practices**, including:
* Making multiple commits to your git repository — Do not wait until your entire program is working before you commit it to your git repository. You should commit your code each time you write and debug a piece of functionality (e.g., after writing and testing code to check the binary formula, after writing and testing the code for extending the length of operands, etc.)
* Properly indenting your code — Recall that indentation is not semantically significant in C, but it makes your code much easier to read.
* Using multiple functions — Do not put all of your code for `maze_solve` in one enormous function. You should use multiple functions, where each function: is Short, does One thing, takes Few parameters, and maintains a single level of Abstraction. In other words, follow the _SOFA_ criteria from COSC 101.
* Include comments — Each function, must be preceded by a short comment that describes what the function does. The body of the function must also include comments; generally, you should include a comment before each conditional statement, loop, and set of statements that perform some calculation. **Do not** include a comment for every line of code, and **do not** simply restate the code.

## Submission instructions
You should **commit and push** all your files to your git repository by the deadlines for Part A and Part B. However, as noted above, do not wait until your entire program is working before you commit it to your git repository; you should commit your code each time you write and debug a piece of functionality. 