#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<limits.h>
#include<omp.h>
#include"graph.h"
#include"timer.h"
#include"dijkstra.h"
#include"heap.h"
#include"alt_heap.h"
#include"alt_dijkstra.h"


/* alternate vision of doing it all in memory so more htread safe */
void parallel_minheap_JUST_TIME_ALT(char *filename){
	int order, size, nPATHS = NUM_PATHS, nThreads;
	//int junk=0; //*junk_ptr=&junk;
	//char *top_ten_name = gen_output_file(filename, PARALLEL_PREFIX"top_ten_", TOP_TEN_FOLDER);
	//char *time_results_name = gen_output_file("_results.txt", PARALLEL_PREFIX"time", TIME_FOLDER);
	//FILE *file;
	//double time = 0.0;


	printf("testing  %s  with serial minheap ", filename);
	/* load graph */
	Graph G = load_graph(filename);
	order = G->nvertices; size = G->nedges;

	/* create heap and heap array */
	Heap main_H;
	if(order < NUM_PATHS){
		nPATHS = order;
	}

	main_H = alt_heap_init(nPATHS);
	
	/* to avoid error */
	//heap_insert(main_H, -40, -3, -3, &junk_ptr);

	printf(" with Graph of order %d and size %d\n", order, size);
	nThreads =  omp_get_max_threads();
	//StartTimer();

	int **distances = (int **) malloc(sizeof(int *) * order);
	Heap *Q_list = (Heap *) malloc(sizeof(Heap) * order);
	Heap *H_list = (Heap*) malloc(sizeof(Heap) * order);
	int ***key_arrays = (int***) malloc(sizeof(int **) * order);

	for (int i = 0; i < order; ++i)
	{
		distances[i] = (int*) malloc(sizeof(int) * order);
		Q_list[i] = alt_heap_init(order);
		H_list[i] = alt_heap_init_TPath(order);
		key_arrays[i] = (int**) malloc(sizeof(int *) * (order));
		for (int j = 1; i < order + 1; ++i)
		{
		Q_list[i]->keys[j] = key_arrays[i][j] = (int*) malloc(sizeof(int) * 4);
		H_list[i]->keys[j][0] = NEG_INF;
		H_list[i]->keys[j][1] = NEG_INF; 
		H_list[i]->keys[j][2] = j; 
		H_list[i]->keys[j][3] = NEG_INF;
		//distances[i];
		}

		/* distances[i] = IINFINITY;
		if(i == v)
			distances[i] = 0;
		heap_insert(Q, distance[i], i, start, &key_array[i]);
		*/ 
		
	}

	for (int i = 0; i < order; ++i)
	{
		free(distances[i]);
		alt_destroy_heap(Q_list[i]);
		alt_destroy_heap(H_list[i]);
		free(key_arrays[i]);

	}

	free(distances);
	free(Q_list);
	free(H_list);
	free(key_arrays);
	alt_destroy_heap(main_H);
	free_graph(G);
	
	//time += GetTimer() / 1000.0;
	return;
}
//	parallel_min_dijkstra(Graph G, int start, Heap top_paths_H,
//		int *distance, Heap Q, int **key_array)
/*
#pragma omp parallel default(none) shared(G, main_H, order, nPATHS, H) num_threads(nThreads)
{
#pragma omp for schedule(dynamic)
	for (int i = 0; i < order; ++i)
	{
		H[i] = heap_init(nPATHS);
		printf("calling dijkstra on vertex %d\n", i);
		free(parallel_min_dijkstra(G, i, H[i]));
	}
}

	for (int i = 0; i < order; ++i)
	{
		heap_merge(main_H, H[i]);
		destroy_heap(H[i]);
	}

	file = fopen(top_ten_name, "a");
	fprintf(file, "%d %d\n", order, size);
	write_main_heap_to_file(file, main_H, nPATHS);
	fclose(file);

	free_graph(G);
	free(top_ten_name);	
	destroy_heap(main_H);
	time += GetTimer() / 1000.0;

	file = fopen(time_results_name, "a");
	fprintf(file, "Parallel method for graph with order %d and size %d\n"
		"%d Threads with time: %lf seconds\n", order, size, nThreads, time);
	fclose(file);
	free(time_results_name);
	printf("Parallel method for graph with order %d and size %d\n"
		"%d Threads with time: %lf seconds\n", order, size,nThreads, time);
	return;
	*/
