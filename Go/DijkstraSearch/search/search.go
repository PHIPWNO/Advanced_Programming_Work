// contains a parallel and serial version of the program
package search

import (
	"fmt"
	"proj3/dijkstra"
	"sync"
)

// serially searches for numPaths longest paths of the paths found by dijkstra
func SerialSearch(filename string, numPaths int) {
	//load graph
	G := dijkstra.LoadGraph(filename)
	//get longest paths
	output := dijkstra.MultipleSourceDijkstra(G, 0, G.NumVertices, numPaths)

	//save the output to file
	dijkstra.SavePathArray(output, fmt.Sprintf("serial_%s", filename))
}

// struct with shared variables for threads
type workerContext struct {
	wg          *sync.WaitGroup         // waitgroup
	G           *dijkstra.Graph         // graph
	numPaths    int                     // number of longest paths to get
	pathChannel chan []dijkstra.PathLen // channel for workers to send their path arrays
}

// Returns pointer to initialized WorkerContext
func NewWorkerContext(G *dijkstra.Graph, numPaths int, pathChannel chan []dijkstra.PathLen) *workerContext {
	wg := new(sync.WaitGroup)
	return &workerContext{wg, G, numPaths, pathChannel}
}

// spawns off nThreads worker threads
func BspSearch(filename string, numPaths int, nThreads int) {
	G := dijkstra.LoadGraph(filename)
	// create channel for reciving path arrays
	pathChannel := make(chan []dijkstra.PathLen)

	//get worker context
	ctx := NewWorkerContext(G, numPaths, pathChannel)

	// static decomp of files
	chunkSize := G.NumVertices / nThreads
	for i := 0; i < nThreads; i++ {
		ctx.wg.Add(1)
		start := i * chunkSize
		var end int
		if i == nThreads-1 {
			end = G.NumVertices
		} else {
			end = (i + 1) * chunkSize
		}
		//start worker function
		go worker(ctx, start, end)
	}
	// make array for global max length paths
	output := make([]dijkstra.PathLen, 0, numPaths)

	// take max from each worker threads result
	for pathArr := range pathChannel {
		output = dijkstra.CombinePathArrays(output, pathArr)
	}
	//save the output to file
	dijkstra.SavePathArray(output, fmt.Sprintf("bsp_%s", filename))
}

// worker function for parallel implementation
func worker(ctx *workerContext, start int, end int) {
	// find shortest paths in workers vertex range
	output := dijkstra.MultipleSourceDijkstra(ctx.G, start, end, ctx.numPaths)
	// send result through channel
	ctx.pathChannel <- output
	// signal done
	ctx.wg.Done()
	//barrier for waiting
	ctx.wg.Wait()
	//thread assigned to starting at 0 closes the channel
	if start == 0 {
		close(ctx.pathChannel)
	}
}
