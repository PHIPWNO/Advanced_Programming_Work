#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<limits.h>
#include"graph.h"
#include"heap.h"
#include"alt_heap.h"
#include"alt_dijkstra.h"


/* returns a sorted list of paths */
Path parallel_min_dijkstra_alt(Graph G, int start, int nTopPaths, Path top_paths){
	int u, v = start, weight, alt;
	Edgenode p;
	struct path_ paths_arr[G->nvertices];
	Path new_path = &(struct path_) {0, -35, -36, -IINFINITY}, path_array[G->nvertices]; 
	

	int distance[G->nvertices];

	Heap top_paths_H = heap_init(nTopPaths);
	heap_insert(top_paths_H, new_path);

	/* priority queue binary minheap */
	Heap Q = heap_init(G->nvertices);

	/* array of key pointers for decreasing priority */
	/* plus one may not be needed */


	for (int i = 0; i < G->nvertices; ++i)
	{
		
		distance[i] = IINFINITY;
		if(i == v)
			distance[i] = 0;
		paths_arr[i].index = i; paths_arr[i].start = i;
		paths_arr[i].end = start; paths_arr[i].key = distance[i];
		
		path_array[i] = &paths_arr[i];
		heap_insert(Q, path_array[i]);
		//print_heap(Q);	
	}
	//printf("ya\n");
	//print_heap(Q);

	while (Q->cur_size > 0){
		
    	/* get spot in queue */
    	new_path = get_heap_front(Q);
		v = new_path->start;
    	p = G->edges[v];
		//printf("got new thing from queue with vertex num %d and key %d \n",
		//new_path->start, new_path->key);
    /* checks v's neighbors */
	while(p != NULL){
		u = p->u;		
		weight = p->weight;
		alt = distance[v] + weight;
		
		if (alt < distance[u]){
			distance[u] = alt;
			//printf("about to subtract %d from %d with index %d \n", 
			//path_array[u]->key - alt, u, path_array[u]->index);
			decrease_key(Q, path_array[u]->index, path_array[u]->key - alt);
			//printf("yo u is %d\n", u);
			//print_heap(Q);
		}
		p = p->next;
    }
	//printf("out of while loop for vertex v=%d\n", v);
	//print_path(new_path);
    	
    	/* for unreachable vertices sets to -1 */
    	if(distance[v] == IINFINITY){
    		distance[v] = -1;
    	}
    	else{
			//printf("about to blind insert for v=%d\n", v);
    		/* tries to insert distance into heap */
			//print_heap(top_paths_H);
    		blind_insert(top_paths_H, new_path);
			//printf("blind inserted for v=%d\n", v);
			//print_heap(Q);
    	}
  }

	//printf("oy bruv am out!\n");
	//print_heap(top_paths_H);
	for (int i = 0; i < nTopPaths; i++)
	{
		//printf("huh? %d\n", i);
		/* makes top paths a list sorted biggest to smallest key */
		new_path = get_heap_front(top_paths_H);
		top_paths[nTopPaths - 1 - i] = (struct path_) {new_path->index,
		new_path->start, new_path->end, new_path->key};
	}
	
	return top_paths;
}