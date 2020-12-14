#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"graph.h"

#define LINE_SIZE 200

/* frees edgenode and all those linked to it */
void free_edgenode_head(Edgenode edge, int degree){
	Edgenode curr = edge, next;
	for (int i = 0; i < degree; ++i)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	return;
}


/* mallocs a graph and its fields */
Graph graph_init(int order, int size){
	Graph G = (Graph) malloc(sizeof(struct graph_));
	int i;
	/* init graph fields */
	G->nvertices = order;
	G->nedges = size;
	G->degree = (int *) malloc(sizeof(int) * (order));
	G->edges = (Edgenode*) malloc(sizeof(Edgenode) * (order));

	for (i=0; i<order; i++) G->degree[i] = 0;
	for (i=0; i<order; i++) G->edges[i] = NULL;
	return G;
}

/* frees a graph and all the edgenodes */
void free_graph(Graph G){
	int i;
	
	for (i = 0; i < G->nvertices; ++i)
	{
		/* frees non null pointers */
		if(G->edges[i] != NULL)
			free_edgenode_head(G->edges[i], G->degree[i]);
	}
	free(G->degree);
	free(G->edges);
	free(G);
	return;
}

/* inserts an edge to the graph */
void insert_edge(Graph G, int v, int u, int weight){
	Edgenode p = (Edgenode) malloc(sizeof(struct edgenode));

	p->weight = weight;
	p->u = u;
	p->next = G->edges[v]; /* get current head of v */

	G->edges[v] = p; /* make new edge head of list at v */
	G->degree[v]++; /* increases outdegree of v */ 
	return;
}


/* loads graph from file assuming # lines are ignored
and first non-comment line has order and size followed by 
size lines of vertex, vertex, edge weight */
Graph load_graph(char *filename){
	char line[LINE_SIZE]; /* buffer for checking for comments */
	bool comment = true; /* for checking if line is a comment */
	int i, order, size, v, u, weight; /* v is current vertex, u is next one*/
	FILE *file = fopen(filename, "r");

	/* ignores comments */
	while(comment){
		fgets(line, LINE_SIZE - 1, file);
		if(*line != '#'){
			comment = false;
		}
	}
	/* gets order and size to create graph */
	sscanf(line, "%d %d", &order, &size);
	Graph G = graph_init(order, size);

	/* loads edges into graph */
	for (i = 0; i < size; ++i)
	{
		fscanf(file, "%d %d %d", &v, &u, &weight);
		insert_edge(G, v, u, weight);
	}
	fclose(file);
	return G;
}

/* prints graph for testing purposes */
void print_graph(Graph G){
  int i;
  Edgenode p;

  for (i=0; i<G->nvertices; i++) {
    printf("%d: ", i);
    p = G->edges[i];
    while (p != NULL) {
      printf(" %d", p->u);
      p = p->next;
    }
    printf("\n");
  }
}


