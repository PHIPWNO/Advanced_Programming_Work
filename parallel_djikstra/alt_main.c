#include<stdlib.h>
#include<stdio.h>
#include"alt_parallel.h"
#include"alt_dijkstra.h"
#include"graph.h"

/* just times and writes top 10 to file for serial version of min heap version */
int main(int argc, char **argv){
	int file_index = 0, order, size, nPaths = 10;
	char *inputs[4] = {"graph4.txt", "graph3.txt", "graph1.txt",
	"graph2.txt"};
	//Path result;

	/* can choose to do big file or just small ones */ 
	
	if(argc > 1){
		int temp = atoi(argv[1]);
		if(temp < 4)
			file_index = temp;
	}
	
	Graph G = load_graph(inputs[file_index]);
	order = G->nvertices; size = G->nedges;

	if(order < nPaths)
		nPaths = order;

	printf("Running on %s with num paths %d\n", inputs[file_index], nPaths);

	//Path top_paths = malloc(sizeof(struct path_) * nPaths);
	struct path_ top_paths[10];

	int sizeDD;

	for (int i = 0; i < order; i++){
	printf("haha i=%d\n", i);
	//top_paths = parallel_min_dijkstra_alt(G, 1, nPaths, top_paths);
	sizeDD = parallel_min_dijkstra_alt(G, i, nPaths, top_paths);
	//printf("hehe i=%d\n", 1);
	/*
	for (int j = 0; j < sizeDD; j++)
	{
		print_path(&top_paths[j]);
	}
	*/

	}

	

/* 
	char *time_results_name = gen_output_file("_results.txt", SERIAL_PREFIX"time", TIME_FOLDER);
	FILE *file = fopen(time_results_name, "w");
	fprintf(file, "Time for Serial minheap method for graphs: ");
	
	for (int i = 0; i < input_size; ++i)
	{
		fprintf(file, "%d, ", i);
	}
	fclose(file);

	for (int i = 0; i < input_size; ++i)
	{
		serial_minheap_JUST_TIME(inputs[i]);
	}

	free(time_results_name);
	*/
	return 0;
}

