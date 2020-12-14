#ifndef ALT_HEAP_H_
#define ALT_HEAP_H_
/* creates a heap and sets first element to zero (this
element is to be ignored) */
Heap alt_heap_init(int max_size);


Heap alt_heap_init_TPath(int max_size);

/* prints elements of heap as list */
void alt_print_heap(Heap H);
/* prints a list of the keys in the heap */
void alt_print_heap_keys(Heap H);

/* destroys a heap */
void alt_destroy_heap(Heap H);


/* inserts to the heap and points key pointer to the 
new key to keep track of
NOTE: This should only be called when the heap is not 
full */
void alt_heap_insert(Heap H, int key_val, int v, 
	int u, int **key_ptr);

/* pushes first element off the heap and updates
the values key_val, v (starting vertex), and u (ending vertex)
point to element taken off the heap */
void alt_get_heap_front(Heap H, int *key_val, int *v, int *u);

/* attempt to insert into the heap without
knowing if full or not. 
Returns 1 if insertion made and 0 if no insertion
because proposed insert was too small 
If heap is full it discards the element taken off*/
int alt_blind_insert(Heap H, int new_key_val, int new_v, 
	int new_u, int **key_ptr);


/* takes all elements from heap 2 and tries to insert them
into heap 1 */
void alt_heap_merge(Heap H1, Heap H2);

#endif /* ALT_HEAP_H_ */
