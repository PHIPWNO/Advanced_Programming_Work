//Package with struct representing graphs using adjacncy list form.
package dijkstra

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
)

/* comment character for graph files */
const commentChar = byte('#')

const filePrefix = "../data/graphs/"

// struct for edge in graph
type edge struct {
	startNode int   /* starting vertex of edge */
	endNode   int   /* number of vertex at the end of connection (not the starting one) */
	weight    int   /* weight of connection */
	nextEdge  *edge /* pointer to next node */
}

// struct for graph
type Graph struct {
	adjList     []*edge /* edges in adjacency list form */
	degree      []int   /* array of outdegree of each vertex */
	NumVertices int     /* number of vertices in graph */
	NumEdges    int     /*number of edges in graph */
}

// method to add an edge to the graph
func (G *Graph) addEdge(startVertex int, endVertex int, weight int) {
	/* create new edge */
	newEdge := &edge{startVertex, endVertex, weight, G.adjList[startVertex]}
	/* make it new head of adj list */
	G.adjList[startVertex] = newEdge
	/* increase outdegree of startNode */
	G.degree[startVertex]++ /* increases outdegree of v */
}

// prints the graph in adjaceny list form
func (G *Graph) Print() {
	fmt.Println("Starting Vertex: End Vertex (edge weight)| ...")
	for vertex, edge := range G.adjList {
		fmt.Printf("%d: ", vertex)
		element := edge
		for element != nil {
			fmt.Printf("%d (%d)| ", element.endNode, element.weight)
			element = element.nextEdge
		}
		fmt.Print("\n")
	}
}

/* creates graph with specified order and size and returns a pointer
to that object */
func createGraph(order int, size int) *Graph {
	degreeArray := make([]int, order)
	adjList := make([]*edge, order)

	return &Graph{adjList, degreeArray, order, size}
}

/* loads a graph from file and returns pointer to it*/
func LoadGraph(fileName string) *Graph {
	fullFileName := fmt.Sprintf("%s%s.txt", filePrefix, fileName)
	// check file exists
	if _, err := os.Stat(fullFileName); os.IsNotExist(err) {
		panic(err)
	}

	/* open file */
	file, err := os.Open(fullFileName)
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()
	scanner := bufio.NewScanner(file)
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
	var order, size int
	//scan until all comments have been read
	for scanner.Scan() {
		line := scanner.Text()
		//break when encountering first non comment
		if line[0] != commentChar {
			line = strings.TrimSpace(line)
			fmt.Sscanf(line, "%d %d", &order, &size)
			break
		}
	}

	// get empty graph
	G := createGraph(order, size)

	var startVertex, endVertex, weight int
	//read in edges
	for scanner.Scan() {
		fmt.Sscanf(scanner.Text(), "%d %d %d", &startVertex, &endVertex, &weight)
		G.addEdge(startVertex, endVertex, weight)
	}
	return G
}
