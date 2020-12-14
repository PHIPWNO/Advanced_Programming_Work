#include<stdio.h>
#include<stdlib.h>
#include"graph.h"
#include"heap.h"
#include <limits.h>
#include"heap.h"
#include"alt_heap.h"

/* creates a heap and sets first element to zero (this
element is to be ignored) */
Heap alt_heap_init(int max_size){
	Heap H = (Heap) malloc(sizeof(struct heap_));
	H->max_size = max_size;
	H->cur_size = 1;
	H->keys = (int**) malloc(sizeof(int*) * (max_size + 1));
	/* mallocs whole array */
	for (int i = 0; i < 1; ++i)
	{
		H->keys[i] = (int*) malloc(sizeof(int) * 4);
		H->keys[i][0] = NEG_INF;
		H->keys[i][1] = NEG_INF; 
		H->keys[i][2] = i; 
		H->keys[i][3] = NEG_INF;
	}
	H->keys[0][2] = -13; 
	return H;
}

Heap alt_heap_init_TPath(int max_size){
	Heap H = (Heap) malloc(sizeof(struct heap_));
	H->max_size = max_size;
	H->cur_size = 1;
	H->keys = (int**) malloc(sizeof(int*) * (max_size + 1));
	/* mallocs whole array */
	for (int i = 0; i < H->max_size + 1; ++i)
	{
		H->keys[i] = (int*) malloc(sizeof(int) * 4);
		H->keys[i][0] = NEG_INF;
		H->keys[i][1] = NEG_INF; 
		H->keys[i][2] = i; 
		H->keys[i][3] = NEG_INF;
	}
	H->keys[0][2] = -13; 
	return H;
}

/* prints elements of heap as list */
void alt_print_heap(Heap H){
	printf("Heap has current size: %d, max size %d\n"
		"Printing 'real elements'\n",
		H->cur_size, H->max_size);
	for (int i = 0; i < H->cur_size; ++i)
	{
		printf("Index %d:, Key:%d, Start Vertex:%d, End Vertex:%d\n",
		 H->keys[i][2], H->keys[i][0], H->keys[i][1], H->keys[i][3]);
	}
	printf("Now printing 'non-existant' elements\n");
	for (int i = H->cur_size; i < H->max_size-1; ++i)
	{
		printf("Index %d:, Key:%d, Start Vertex:%d, End Vertex:%d\n",
		 H->keys[i][2], H->keys[i][0], H->keys[i][1], H->keys[i][3]);
	}
	return;
}

/* prints a list of the keys in the heap */
void alt_print_heap_keys(Heap H){
	printf("Heap has current size: %d, max size %d\n"
		"Printing 'real keys'\n",
		H->cur_size, H->max_size);
	for (int i = 0; i < H->cur_size; ++i)
	{
		printf("Key:%d\n", H->keys[i][0]);
	}
	printf("Now printing 'non-existant' elements\n");
	for (int i = H->cur_size; i < H->max_size+1; ++i)
	{
		printf("Key:%d\n", H->keys[i][0]);
	}
	return;
}

/* destroys a heap */
void alt_destroy_heap(Heap H){
	for (int i = 0; i < H->max_size+1; i++)
	{
		free(H->keys[i]);
	}
	free(H->keys);
	free(H);
	return;
}


/* inserts to the heap and points key pointer to the 
new key to keep track of
NOTE: This should only be called when the heap is not 
full */
void alt_heap_insert(Heap H, int key_val, int v, int u, int **key_ptr){
	if(H->cur_size-1 == H->max_size){
		printf("YOU ARE TRYING TO INSERT INTO A FULL HEAP!\n");
		return;
	}
	*key_ptr[0] = key_val; *key_ptr[1] = v; *key_ptr[3] = u;
	 *key_ptr[2] = H->cur_size;
	/* always assume heap not at max capacity */
	H->keys[H->cur_size] = *key_ptr;
	H->cur_size++;
	percolate_up(H);
}

/* pushes first element off the heap and updates
the values key_val, v (starting vertex), and u (ending vertex)
point to element taken off the heap */
void alt_get_heap_front(Heap H, int *key_val, int *v, int *u){
	int *output = H->keys[1];
	H->keys[1] = H->keys[H->cur_size-1];
	H->keys[1][2] = 1;
	H->cur_size--;
	percolate_down(H, 1);
	*key_val = output[0]; *v = output[1]; *u = output[3];
	H->keys[H->cur_size+1]= output;
	return;
}

/* attempt to insert into the heap without
knowing if full or not. 
Returns 1 if insertion made and 0 if no insertion
because proposed insert was too small 
If heap is full it discards the element taken off*/
int alt_blind_insert(Heap H, int new_key_val, int new_v, int new_u, int **key_ptr){
	//printf("blind insert\n");
	//printf("params are %d, %d, and %p\n", new_key_val, new_v, key_ptr);
	if(new_key_val <= H->keys[1][0]){
		/* if the element you are trying to insert is smaller than
		the front of the heap then no insertion made */
		return 0;
	}
	if(H->cur_size-1 >= H->max_size){
		/* For when stack is full */
		int junk1;
		alt_get_heap_front(H, &junk1, &junk1, &junk1);
	}
	/* inserts */
	alt_heap_insert(H, new_key_val, new_v, new_u, key_ptr);
	return 1;
}


/* takes all elements from heap 2 and tries to insert them
into heap 1 */
void alt_heap_merge(Heap H1, Heap H2){
	int key_val, v, u, junk, *junk_ptr = &junk, H2_size = H2->cur_size;
	for (int i = 1; i < H2_size; ++i)
	{
		/* take from second heap and attempt to push
		to first heap */
		alt_get_heap_front(H2, &key_val, &v, &u);
		alt_blind_insert(H1, key_val, v, u, &junk_ptr);
	}
	return;
}


/* writes heap to a list of keys 
void heap_to_list(Heap H, int *arr){
	for (int i = 1; i < H->cur_size; ++i)
	{
		arr[i-1] = H->keys[i][0];
	}
	return;

}
*/

