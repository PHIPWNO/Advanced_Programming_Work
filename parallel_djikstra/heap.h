#ifndef HEAP_H_
#define HEAP_H_

#define NEG_INF INT_MIN/2
#define NUM_PATHS 10
#define IINFINITY INT_MAX/4

#include<stdbool.h>
#include"graph.h"

typedef struct heap_{
	int cur_size; /* current size of heap (DOES NOT COUNT FIRST ELEMENT OF LIST) */
	int max_size; /* how many elements the heap can hold */
	Path *paths; /* array of Path structs */
} *Heap;

/* creates a heap, sets first element to NULL (this
element is to be ignored) and mallocs node array of size
max_size + 1*/
Heap heap_init(int max_size);


/* prints elements of heap as list */
void print_heap(Heap H);

/* pushes first element off the heap and updates
the values key_val, v (starting vertex), and u (ending vertex)
point to element taken off the heap */
Path get_heap_front(Heap H);

/* inserts new node into the heap
NOTE: This should only be called when the heap is not full */
void heap_insert(Heap H, Path new_path);

/* decreases the key value to the change amount */
void decrease_key(Heap H, int index, int change);


/* attempt to insert into the heap without
knowing if full or not. 
Returns 1 if insertion made and 0 if no insertion
because proposed insert was too small 
If heap is full it discards the element taken off*/
int blind_insert(Heap H, Path new_path);


#endif /* HEAP_H_ */
