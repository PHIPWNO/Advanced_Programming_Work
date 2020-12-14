#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<limits.h>
#include"graph.h"
#include"heap.h"
#include"alt_heap.h"
#include"alt_dijkstra.h"


/* returns a sorted list of paths */
Path* parallel_min_dijkstra_alt(Graph G, int start, int nTopPaths){
	int u, v = start, weight, alt;
	Edgenode p;
	struct path_ paths_arr[G->nvertices];
	Path new_path = NULL, path_array[G->nvertices]; 
	Path *top_paths = malloc(sizeof(Path) * nTopPaths);

	int distance[G->nvertices];

	Heap top_paths_H = heap_init(nTopPaths);

	/* priority queue binary minheap */
	Heap Q = heap_init(G->nvertices);

	/* array of key pointers for decreasing priority */
	/* plus one may not be needed */


	for (int i = 0; i < G->nvertices; ++i)
	{
		
		distance[i] = IINFINITY;
		if(i == v)
			distance[i] = 0;
		paths_arr[i].index = i; paths_arr[i].start = start;
		paths_arr[i].end = i; paths_arr[i].key = distance[i];
		
		path_array[i] = &paths_arr[i];
		heap_insert(Q, path_array[i]);
		//print_heap(Q);	
	}
	printf("ya\n");
	print_heap(Q);

	while (Q->cur_size > 1){
		
    	/* get spot in queue */
    	new_path = get_heap_front(Q);
		v = new_path->start;
    	p = G->edges[v];
		printf("got new thing from queue \n");
    /* checks v's neighbors */
	while(p != NULL){
		u = p->u;		
		weight = p->weight;
		alt = distance[v] + weight;
		
		if (alt < distance[u]){
			distance[u] = alt;
			printf("about to subtract %d from %d\n", path_array[u]->key - alt, u);
			decrease_key(Q, path_array[u]->index, path_array[u]->key - alt);
			printf("yo u is %d\n", u);
			print_heap(Q);
		}
		p = p->next;
    }
	printf("out of while loop for vertex v=%d\n", v);
	print_heap(Q);
    	
    	/* for unreachable vertices sets to -1 */
    	if(distance[v] == IINFINITY){
    		distance[v] = -1;
    	}
    	else{
			printf("about to blind insert for v=%d\n", v);
    		/* tries to insert distance into heap */
			print_heap(top_paths_H);
    		blind_insert(top_paths_H, new_path);
			printf("blind inserted for v=%d\n", v);
    	}
  }

	for (int i = 0; i < nTopPaths; i++)
	{
		/* makes top paths a list sorted biggest to smallest key */
		top_paths[nTopPaths - 1 - i] = get_heap_front(top_paths_H);
	}
	
	return top_paths;
}