# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -O -Wall -Wfatal-errors -std=c99

# define the C source files
SRCS = heap.c alt_main.c graph.c alt_dijkstra.c #alt_parallel.c

# define the C object files 
OBJS = $(SRCS:.c=.o)

# define the C source files
SRCS2 = openmp_test.c $(LOCATION1)timer.c 

# define the C object files 
OBJS2 = $(SRCS2:.c=.o)

# define the executable file 
MAIN = main

TEST = Test

.PHONY: depend clean

all: $(MAIN) $(TEST)
	@echo  Simple compiler named Test has been compiled

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

# $(TEST): $(OBJS2) 
#	$(CC) -Xpreprocessor -fopenmp $(CFLAGS) $(INCLUDES) -o $(TEST) $(OBJS2) $(LFLAGS) $(LIBS) -lomp

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o

# depend: $(SRCS)
#	makedepend $(INCLUDES) $^
