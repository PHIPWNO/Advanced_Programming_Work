#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <limits.h>
#include"heap.h"
#include"graph.h"


/* creates a heap, sets first element to NULL (this
element is to be ignored) and mallocs node array of size
max_size + 1*/
Heap heap_init(int max_size){
	Heap H = (Heap) malloc(sizeof(struct heap_));
	H->max_size = max_size;
	H->cur_size = 0;
	H->paths = (Path *) malloc(sizeof(Path) * (max_size + 1));
	/* mallocs whole array */
	H->paths[0] = NULL; 
	return H;
}


/* prints elements of heap */
void print_heap(Heap H){
	printf("Heap has current size: %d, max size %d\n"
		"Printing 'real elements'\n",
		H->cur_size, H->max_size);
	for (int i = 1; i < H->cur_size+1; ++i)
	{
		print_path(H->paths[i]);
	}
	printf("Done printing the heap\n\n");
	return;
}


/* fress heap from memory */
void destroy_heap(Heap H){
	for (int i = 1; i < H->cur_size; i++)
	{
		free(H->paths[i]);
	}
	free(H->paths);
	free(H);
	return;
}

/* finds smaller child index's two children in array */
int min_child(Heap H, int index){
	int len = H->cur_size;
	if(index * 2 + 1 > len)
		return index * 2;

	if(H->paths[index * 2]->key < H->paths[index * 2 + 1]->key)
		return index * 2;
	return index * 2 + 1;
}

/* moves value at index down to proper place to maintain 
heap structure */
void percolate_down(Heap H, int index){
	int min_kid, len = H->cur_size;
	Path temp;
	while(index * 2 <= len){
		min_kid = min_child(H, index);
		if(H->paths[index]->key > H->paths[min_kid]->key){
			temp = H->paths[index];
    		H->paths[index] = H->paths[min_kid];
    		H->paths[min_kid] = temp;
			/* swaps indices */
    		H->paths[index]->index = index;
    		H->paths[min_kid]->index = min_kid;
    	}
    	index = min_kid;
	}
}

/* moves value at end of heap up to proper place to maintain 
heap structure */
void percolate_up(Heap H){
	int len = H->cur_size;
	Path temp;
	while(len/2 > 0){
		if(H->paths[len]->key < H->paths[len/2]->key){
			temp = H->paths[len]; 
    		H->paths[len] = H->paths[len/2]; 
    		H->paths[len/2] = temp;
			/* swap indices */
    		H->paths[len]->index = len;
    		H->paths[len/2]->index = len/2;
    	}
		len /= 2;
	}
	return;
}


/* inserts new node into the heap
NOTE: This should only be called when the heap is not full */
void heap_insert(Heap H, Path new_path){
	H->paths[H->cur_size+1] = new_path;
	H->paths[H->cur_size+1]->index = H->cur_size+1;
	H->cur_size++;
	//print_heap(H);
	percolate_up(H);
}

/* pushes first element off the heap and percolates down
to maintain valid structure */
Path get_heap_front(Heap H){
	Path output = H->paths[1];
	H->paths[1] = H->paths[H->cur_size];
	H->paths[1]->index = 1;
	H->paths[H->cur_size] = NULL;
	H->cur_size--;
	percolate_down(H, 1);
	return output;
}


/* decreases the key value to the change amount */
void decrease_key(Heap H, int index, int change){
	H->paths[index]->key -= change;
	Path temp;
	while(index > 1){
		//print_path(H->paths[index]);
		//print_path(H->paths[index/2]);
		if(H->paths[index]->key < H->paths[index/2]->key){
			//printf("first is less than second!\n");
			temp = H->paths[index]; 
    		H->paths[index] = H->paths[index/2]; 
    		H->paths[index/2] = temp;
    		/* swtich indices */
    		H->paths[index]->index = index;
    		H->paths[index/2]->index = index/2;
    	}
		index /= 2;
	}
	//printf("index is now %d\n", index);
	return;
}


/* attempt to insert into the heap without
knowing if full or not. 
Returns 1 if insertion made and 0 if no insertion
because proposed insert was too small 
If heap is full it discards the element taken off*/
int blind_insert(Heap H, Path new_path){
	//printf("blind insert path with key %d\n", new_path->key);
	//printf("params are %d, %d, and %p\n", new_key_val, new_v, key_ptr);
	if(new_path->key <= H->paths[1]->key){
		/* if the element you are trying to insert is smaller than
		the front of the heap then no insertion made */
		return 0;
	}
	if(H->cur_size >= H->max_size){
		/* For when stack is full */
		get_heap_front(H);
	}
	/* inserts */
	heap_insert(H, new_path);
	return 1;
}
