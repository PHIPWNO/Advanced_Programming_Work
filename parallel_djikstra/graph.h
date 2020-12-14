#ifndef GRAPH_H_
#define GRAPH_H_

typedef struct path_{
  int index; /* index of the */
  int start; /* starting vertex */
  int end; /* ending vertex */
  int key; /* key value */
} *Path;

typedef struct edgenode{
  int u;                  /* number of vertex at the end of connection (not the starting one) */
  int weight;             /* weight of connection */
  struct edgenode *next;  /* pointer to next node */
} *Edgenode;


typedef struct graph_{
  Edgenode *edges;  /* array of edgenodes, each edgenode v can be thought of as head of 
  					   linked list of edgenodes representing vertices u where you can go
  					    from v to u */
  int *degree;      /* array of outdegree of each vertex */
  int nvertices;    /* number of vertices in graph */
  int nedges;       /*number of edges in graph */
} *Graph;


/* loads graph from file assuming # lines are ignored
and first non-comment line has order and size followed by 
size lines of vertex, vertex, edge weight */
Graph load_graph(char *filename);

/* frees a graph and all the edgenodes */
void free_graph(Graph G);

/* prints graph for testing purposes */
void print_graph(Graph G);

#endif /* GRAPH_H_ */
