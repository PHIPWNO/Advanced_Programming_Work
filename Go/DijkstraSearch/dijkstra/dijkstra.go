// Contains functions for calling dijkstra and combining outputs of a single call
package dijkstra

import (
	"fmt"
	"math"
	"os"
)

const infinity = int(math.MaxInt64) / 2

const outputDir = "../data/output/"

// calls dijkstra and of the shortest paths found using dijkstra returns the numPaths longest paths sorted from biggest to smallest
func (G *Graph) Dijkstra(startVertex int, numPaths int) []PathLen {

	//make priority queue for dijkstra
	Q := CreateHeap(G.NumVertices)
	// make set to keep track of already visited vertices
	visited := make(map[int]bool)
	// map keeping track of shortest paths (i.e. distance[v] is shortest path to v)
	distances := make(map[int]int, G.NumVertices)
	//insert all nodes into the queue
	for i := 0; i < G.NumVertices; i++ {
		// if not starting vertex insert with infinity distance
		if i != startVertex {
			Q.DijkstraInsert(startVertex, i, infinity)
			distances[i] = infinity
		} else {
			Q.DijkstraInsert(startVertex, i, 0)
			distances[i] = 0
		}
	}

	//Make Max-heap for tracking longest shorest paths
	longestPaths := CreateHeap(numPaths)

	//run until queue is empty
	for Q.size > 0 {
		// Extract min from queue
		u := Q.ExtractMin().EndVertex
		// mark vertex as visited
		visited[u] = true
		// visit vertex's neighbors
		edge := G.adjList[u]
		for i := 0; i < G.degree[u]; i++ {
			v := edge.endNode
			// only attempt to relax if not already visited
			if _, in := visited[v]; !in {
				// get weight from start to u + weight from u to v
				newPathLen := distances[u] + edge.weight
				if distances[v] > newPathLen {
					Q.DecreaseKey(v, newPathLen)
					distances[v] = newPathLen
				}
			}
			//move on to next edge
			edge = edge.nextEdge
		}
		//only add path if it has been changed from infinity
		if distances[u] != infinity {
			longestPaths.PathInsert(startVertex, u, distances[u])
		}

	}
	//create output slice
	output := make([]PathLen, longestPaths.size)

	//returned sorted output
	for i := 0; i < len(output); i++ {
		output[len(output)-i-1] = *longestPaths.ExtractMin()
	}
	return output
}

/* takes two path arrays sorted from biggest to smallest and returns
an array with the biggest n elements where n = max(len(a), len(b))*/
func CombinePathArrays(a []PathLen, b []PathLen) []PathLen {
	var newSize int
	if len(a) > len(b) {
		newSize = len(a)
	} else {
		newSize = len(b)
	}
	output := make([]PathLen, newSize)
	aIndex := 0
	bIndex := 0
	//merge the two lists
	for i := 0; i < newSize; i++ {
		//check if already at end of a or if current spot in b is bigger
		if aIndex >= len(a) || a[aIndex].Weight <= b[bIndex].Weight {
			output[i] = b[bIndex]
			bIndex++
		} else if bIndex >= len(b) || b[bIndex].Weight < a[aIndex].Weight {
			//check if already at end of b or if current spot in a is bigger
			output[i] = a[aIndex]
			aIndex++
		}
	}
	return output
}

// Runs dijkstra from startingVertex to endingVertex (non-inclusive)
// returns longest numPaths sorted longest to shortest
func MultipleSourceDijkstra(G *Graph, startingVertex int, endingVertex int, numPaths int) []PathLen {
	output := make([]PathLen, 0, numPaths)
	// get dijkstra result for each vertex and merge them
	for i := startingVertex; i < endingVertex; i++ {
		output = CombinePathArrays(output, G.Dijkstra(i, numPaths))
	}
	return output
}

/* takes a path array and saves it to a text file */
func SavePathArray(paths []PathLen, filename string) {
	f, err := os.Create(fmt.Sprintf("%s%s_results.txt", outputDir, filename))
	if err != nil {
		panic(err)
	}
	defer f.Close()

	for _, val := range paths {
		fmt.Fprintf(f, "(%d,%d,%d)\n", val.StartVertex, val.EndVertex, val.Weight)
	}
}
