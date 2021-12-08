CC = clang
CFLAGS = -g -Wall -std=c11 -fPIC
SRCS = stack.c maze.c 
OBJS = stack.o maze.o 
HDRS = stack.h maze.h 
LIBS = libmaze.so
BINS = test_stack test_maze test_solve
.PHONY : clean

all: $(LIBS) $(BINS)

test_%: test_%.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(BINS) $(LIBS)

libmaze.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^