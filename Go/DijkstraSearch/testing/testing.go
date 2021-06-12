//Package for testing some of the components of the program
package main

import (
	"fmt"
	"math/rand"
	"os"
	"proj3/dijkstra"
)

const usage = `Usage: test test_type
test_type = (load) to test loading sample graph from file,
		(dijkstra) to test a single instance of dijkstra runs correctly,
		(HeapSort) to test heap sorts correctly,
		(HeapPriority) to test updating priority for the heap works correctly`

const fileName = "../data/graphs/dijkstra_test.txt"

//tests single instance of dijkstra call
func testSingleDijkstra() {
	G := dijkstra.LoadGraph(fileName)
	output := G.Dijkstra(0, 9)
	expectedOutput := [][]int{
		{0, 4, 21},
		{0, 3, 19},
		{0, 8, 14},
		{0, 2, 12},
		{0, 5, 11},
		{0, 6, 9},
		{0, 7, 8},
		{0, 1, 4},
		{0, 0, 0},
	}
	fmt.Println("Expecting", expectedOutput)
	success := true
	fmt.Println("Got")
	for i := 0; i < len(output); i++ {
		output[i].Print()
		start := output[i].StartVertex
		end := output[i].EndVertex
		weight := output[i].Weight
		if start != expectedOutput[i][0] || end != expectedOutput[i][1] || weight != expectedOutput[i][2] {
			success = false
		}
	}
	if success {
		fmt.Println("Output Matches")
	} else {
		fmt.Println("Output DOES NOT MATCH")
	}
}

// tests that updating an element in the heap works
func heapTest() {
	fmt.Println("Testing that update key works")
	heap := dijkstra.CreateHeap(10)
	randInts2 := rand.Perm(10)
	for i, k := range randInts2 {
		heap.DijkstraInsert(0, i, k)
	}
	fmt.Println("Printing Current Heap")
	heap.Print()
	fmt.Printf("Updating path to vertex 9 to -999, verify it is now the first element of the queue\n")
	heap.DecreaseKey(9, -999)
	heap.Print()
	if heap.IsValid() {
		fmt.Println("Heap still has valid structure after modification!")
	} else {
		fmt.Println("Error! Heap structure invalid")
	}
	fmt.Printf("Updating path to vertex 3 to -99009, verify it is now the first element of the queue\n")
	heap.DecreaseKey(3, -99009)
	if heap.IsValid() {
		fmt.Println("Heap still has valid structure after modification!")
	} else {
		fmt.Println("Error! Heap structure invalid")
	}
	heap.Print()
}

// tests that the heap correctly sorts the n integers
func heapSortTest(n int) {
	fmt.Printf("Testing on %d elements\n", n)
	randInts := rand.Perm(n)
	minHeap := dijkstra.CreateHeap(n)
	maxHeap := dijkstra.CreateHeap(n)
	for _, k := range randInts {
		minHeap.DijkstraInsert(0, 0, k)
		maxHeap.DijkstraInsert(0, 0, -k)
	}
	//check heap structure is valid
	if minHeap.IsValid() && maxHeap.IsValid() {
		fmt.Println("Heaps have valid structure after initial inserts!")
	} else {
		fmt.Println("Error! Heap structure invalid")
	}

	success := true
	for i := 0; i < len(randInts); i++ {
		min := minHeap.ExtractMin().Weight
		if min != i {
			success = false
		}
		max := -maxHeap.ExtractMin().Weight
		if max != n-i-1 {
			success = false
		}
		// checks 10 times
		if i%(n/10) == 0 {
			if minHeap.IsValid() && maxHeap.IsValid() {
				fmt.Printf("Heaps still have valid structure after %d removals!\n", i)
			} else {
				fmt.Println("Error! Heap structure is now invalid")
			}
		}
	}
	if success {
		fmt.Println("Heap sorts successfully as min and max heap!")
	} else {
		fmt.Println("Heap is not sorting sucessfully :(")
	}
}

// main function
func main() {
	args := os.Args[1:]
	if len(args) == 0 {
		fmt.Println(usage)
		return
	}
	action := args[0]
	if action == "load" {
		fmt.Printf("Testing %s is correctly loaded into a graph\n", fileName)
		G := dijkstra.LoadGraph(fileName)
		G.Print()
	} else if action == "dijkstra" {
		fmt.Printf("Testing dijkstra on %s starting from vertex 0\n", fileName)
		testSingleDijkstra()
	} else if action == "HeapSort" {
		heapSortTest(10000)
	} else if action == "HeapPriority" {
		heapTest()
	}
}
