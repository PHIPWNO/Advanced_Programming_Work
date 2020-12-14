# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -O -Wall -Werror -Wfatal-errors -std=c99

# define any directories containing header files other than /usr/include
INCLUDES = -I/Users/philiposullivan/OneDrive/Documents/UChicago/CS_Masters/Fall_Quarter/Advanced_Programming/Homework/homework-6-PHIPWNO/

# define library paths in addition to /usr/lib
LFLAGS = -L/Users/philiposullivan/OneDrive/Documents/UChicago/CS_Masters/Fall_Quarter/Advanced_Programming/Homework/homework-6-PHIPWNO/

# define any libraries to link into executable:
LIBS = -lm

# defines a location for other files
LOCATION1 = /Users/philiposullivan/OneDrive/Documents/UChicago/CS_Masters/Fall_Quarter/Advanced_Programming/Homework/homework-6-PHIPWNO/

# define the C source files
SRCS = alt_heap.c alt_parallel.c alt_main.c $(LOCATION1)heap.c $(LOCATION1)graph.c

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
	$(CC) -Xpreprocessor -fopenmp $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS) -lomp

$(TEST): $(OBJS2) 
	$(CC) -Xpreprocessor -fopenmp $(CFLAGS) $(INCLUDES) -o $(TEST) $(OBJS2) $(LFLAGS) $(LIBS) -lomp

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
