//contains main function for running sequential or parallel PathSearch
package main

import (
	"fmt"
	"os"
	"proj3/search"
	"strconv"
	"time"
)

const usage = `Usage: pathSearch graph_file num_paths [num_threads]
graph_file = name of text file containing graph (DO NOT INCLUDE RELATIVE PATH OR .txt ENDING)
num_paths = the number of longest Dijkstra path's to find (If the graph has lass edges than this it will return the number of edges in the graph)
[num_threads] = optional parameter specifying the number of threads. If this parameter
	 is passed the program will be run in parallel. If not then the program will be run sequentially`

// main function for calling either sequential or parallel implementation of Dijkstra Path search
func main() {
	args := os.Args[1:]
	if len(args) != 2 && len(args) != 3 {
		fmt.Println(usage)
		return
	}
	filename := args[0]

	numPaths, err := strconv.Atoi(args[1])
	if err != nil {
		panic(err)
	}

	start := time.Now()

	if len(args) == 2 {
		search.SerialSearch(filename, numPaths)
	} else {
		numThreads, err := strconv.Atoi(args[2])
		if err != nil {
			panic(err)
		}
		search.BspSearch(filename, numPaths, numThreads)
	}
	fmt.Printf("%.2f\n", time.Since(start).Seconds())
}
