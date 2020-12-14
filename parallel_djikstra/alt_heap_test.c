#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include"heap.h"

/* for qsorting random int list */
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}


/* Returns true if arr represents a min heap :
 https://www.geeksforgeeks.org/how-to-check-if-a-given-array-
 	represents-a-binary-heap/ */ 
int isMinHeap(int arr[], int i, int n)
{
    // If a leaf node
    if (i >= (n - 2) / 2)
        return 1;
 
    // If an internal node and is 
    // greater than its children,
    // and same is recursively 
    // true for the children
    if (arr[i] <= arr[2 * i + 1] && 
        arr[i] <= arr[2 * i + 2]
        && isMinHeap(arr, 2 * i + 1, n)
        && isMinHeap(arr, 2 * i + 2, n))
        return 1;
 
    return 0;
}


/* tests heap works by comparing to randomly generated sorted integer list */
int main(int argc, char **argv){
	int numInts = 200, r, output, junk, key_status = 1, index_status = 1,
	key_array_order = 1;
	srand(time(NULL));
	char *index_check[2] = { 
		"The heap indices were all correct! Heap indices works!\n",
		"Sorry something went wrong with the indexes and they do not match!\n"};

	char *minheap_check[2] = 
		{"The Heap is still a valid minheap!\n",
		 "The Heap is NO LONGER a valid minheap!\n"};




	/* optional heap size input */
	if(argc > 1)
		numInts = atoi(argv[1]);
	
	Heap H = alt_heap_init(numInts);
	printf("Testing Heap with %d random integers\n", numInts);

	/* array of random ints to insert/compare after sorting*/
	int *random_list = (int*) malloc(sizeof(int)* numInts);
	/* array for checking if heap is valid */
	int *heap_as_list = (int*) malloc(sizeof(int)* numInts);
	/* array of key pointers */
	int **key_array = (int**) malloc(sizeof(int *) * numInts);

	/* init random int array and insert into heap*/
	for (int i = 0; i < numInts; ++i)
	{
		r = rand();
		random_list[i] = r;
		heap_insert(H, r, i, i, &key_array[i]);
	}

	/* sort array to compare to heap later */
	qsort(random_list, numInts, sizeof(int), cmpfunc);


	/* checks array elements are properly indexed */
	for (int i = 0; i < numInts; ++i)
	{
		//printf("for i=%d the index is %d \n", i, H->keys[i][2]);
		if(H->keys[i][2] != i){
			printf("FOR i = %d INDEX IS WRONG!\n", i);
			index_status = 0;
		}
	}

	/* prints message on whether or not index of heap was correct */
	index_status ? 
		printf("%s", index_check[0]) : printf("%s", index_check[1]);

	/* writes heap to array*/
	heap_to_list(H, heap_as_list);

	/* prints message on whether or not heap is valid */
	isMinHeap(heap_as_list, 0, H->cur_size) ?
		printf("%s", minheap_check[0]) : printf("%s", minheap_check[1]);

	/* checks if keys match */
	for (int i = 0; i < numInts; ++i)
	{
		r = random_list[i];
		get_heap_front(H, &output, &junk, &junk);
		if(output != r){
			printf("THE KEYS DO NOT MATCH FOR i = %d\n", i);
			key_status = 0;
		}

	}


	/* */
	if(key_status)
		printf("They all matched! Heap works for inserts!\n");
	else
		printf("Sorry something went wrong!\n");

	printf("Now testing decrease key operation\n");

	for (int i = 0; i < numInts; ++i)
	{
		r = rand();
		random_list[i] = r;
		heap_insert(H, r, i, i, &key_array[i]);
		if(key_array[i][1] != i){
			printf("Key array order was not preserverd!\n");
			key_array_order = 0;
		}
	}

	if(key_array_order)
		printf("Key array order was preserved!\n");
	else
		printf("Something went wrong with Key array order!\n");

	for (int i = 0; i < numInts; ++i)
	{
		//printf("vertex is %d and i is %d\n", key_array[i][2], i);
		r = rand();
		random_list[i] -= r;
		decrease_key(H, key_array[i][2], r);
		if(key_array[i][1] != i){
			printf("Key array order was not preserverd!\n");
			key_array_order = 0;
		}
	}
	
	if(key_array_order)
		printf("Key array order was preserved after decreases!\n");
	else
		printf("Something went wrong with Key array order after decreases!\n");

	/* writes heap to array*/
	heap_to_list(H, heap_as_list);

	/* checks if still valid */
	isMinHeap(heap_as_list, 0, H->cur_size) ? printf("It is still a valid minheap"
		" after decreases\n") : printf("it is no longer a valid minheap after decreases");

	destroy_heap(H);
	free(key_array);
	free(random_list);
	free(heap_as_list);
	return 0;
	
}
