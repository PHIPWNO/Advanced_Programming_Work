//contains special min heap for implementing dijkstra with priority queue
package dijkstra

import "fmt"

// queue element of start vertex, end vertex, and path length
type PathLen struct {
	StartVertex int // starting vertex
	EndVertex   int // end vertex
	Weight      int // weight, this is the key value
}

//Prints PathLen object
func (path *PathLen) Print() {
	fmt.Printf("Start:%d\tEnd:%d\t%d\n", path.StartVertex, path.EndVertex, path.Weight)
}

// priority queue for implementing dijkstra
type PriorityQueue struct {
	maxSize int         //maximum number of elements queue can hold
	size    int         // current size of the queue
	array   []*PathLen  //slice of actual elements
	indices map[int]int //array mapping end Vertices to their index in the queue's underlying array
}

//finds the smaller child of index's two children
func (Q *PriorityQueue) minChild(index int) int {
	if index*2+1 > Q.size {
		return index * 2
	} else if Q.array[index*2].Weight < Q.array[index*2+1].Weight {
		// if left child < right child
		return index * 2
	} else {
		return index*2 + 1
	}
}

// moves value at the index down to the first location which preserves heap invariant
func (Q *PriorityQueue) heapDown(index int) {
	// moves the value at index down to a proper location
	for index*2 <= Q.size {
		minKid := Q.minChild(index)
		//Swaps if current index is bigger than smallest child
		if Q.array[index].Weight > Q.array[minKid].Weight {
			indexElement := Q.array[index].EndVertex
			minKidElement := Q.array[minKid].EndVertex
			// swap indices
			Q.indices[indexElement], Q.indices[minKidElement] = Q.indices[minKidElement], Q.indices[indexElement]
			// swap elements in array
			Q.array[index], Q.array[minKid] = Q.array[minKid], Q.array[index]
		}
		index = minKid
	}
}

// moves value at the index up to the first location which preserves heap invariant
func (Q *PriorityQueue) heapUp(index int) {
	// moves the value at index up to a proper location
	for index/2 > 0 {
		child := index / 2

		if Q.array[index].Weight < Q.array[child].Weight {
			indexElement := Q.array[index].EndVertex
			minKidElement := Q.array[child].EndVertex
			// swap indices
			Q.indices[indexElement], Q.indices[minKidElement] = Q.indices[minKidElement], Q.indices[indexElement]
			// swap elements in array
			Q.array[index], Q.array[child] = Q.array[child], Q.array[index]
		}
		index = child
	}
}

// insert new element into the heap for dijkstra call, meaning it never checks size
func (Q *PriorityQueue) DijkstraInsert(startVertex int, endVertex int, weight int) {
	Q.size++
	newElement := &PathLen{startVertex, endVertex, weight}
	Q.array[Q.size] = newElement
	Q.indices[endVertex] = Q.size

	// moves the value at index up to a proper location if not inserting into empty heap
	Q.heapUp(Q.size)
}

// insert new element into the heap tracking the longest paths, meaning sometimes
// it won't insert if heap is full and path too short
func (Q *PriorityQueue) PathInsert(startVertex int, endVertex int, weight int) {
	// check if already full
	if Q.size == Q.maxSize {
		// if full and proposed insert is less than queue's min don't insert since
		// it won't be larger than anything already in the queue
		if weight <= Q.array[1].Weight {
			return
		} else {
			// else remove the smallest before inserting
			Q.ExtractMin()
		}
	}
	Q.size++
	newElement := &PathLen{startVertex, endVertex, weight}
	Q.array[Q.size] = newElement
	Q.indices[endVertex] = Q.size

	// moves the value at index up to a proper location if not inserting into empty heap
	Q.heapUp(Q.size)
}

// extracts min element and fixes the heap
func (Q *PriorityQueue) ExtractMin() *PathLen {
	// extracts the min from the heap (remember 0 element is nil)
	output := Q.array[1]

	Q.array[1] = Q.array[Q.size]
	Q.indices[Q.array[1].EndVertex] = 1
	Q.size--
	// repairs heap
	Q.heapDown(1)
	return output
}

// gets the weight of path with specific endVertex
func (Q *PriorityQueue) GetPathLen(endVertex int) int {
	return Q.array[Q.indices[endVertex]].Weight
}

// decreases the value of that element's key and update heap
func (Q *PriorityQueue) DecreaseKey(endVertex int, newKey int) {
	index := Q.indices[endVertex]
	Q.array[index].Weight = newKey
	// moves index up to new valid position
	Q.heapUp(index)
}

// creates priority min queue and returns a pointer to it
func CreateHeap(maxSize int) *PriorityQueue {
	array := make([]*PathLen, maxSize+1)
	indices := make(map[int]int)
	return &PriorityQueue{maxSize, 0, array, indices}
}

//prints values in the queue
func (Q *PriorityQueue) Print() {
	for i := 1; i <= Q.size; i++ {
		startVertex := Q.array[i].StartVertex
		endVertex := Q.array[i].EndVertex
		weight := Q.array[i].Weight
		arrayIndex := Q.indices[Q.array[i].EndVertex]
		fmt.Printf("Vertex %d to %d: weight %d, array index %d\n", startVertex, endVertex, weight, arrayIndex)
		//fmt.Println(*Q.array[i], Q.indices[Q.array[i].EndVertex], *Q.array[Q.indices[Q.array[i].EndVertex]])
	}
}

//checks that heap is still valid'
//https://www.geeksforgeeks.org/how-to-check-if-a-given-array-represents-a-binary-heap/
func (Q *PriorityQueue) IsValid() bool {
	status := true
	for i := 0; i < (Q.size-2)/2; i++ {
		index := i + 1
		if Q.array[2*index].Weight < Q.array[index].Weight {
			fmt.Printf("Child %d of Node %d is greater", 2*index+1, index)
			status = false
			break

		}
		if 2*index+1 <= Q.size && Q.array[2*index+1].Weight < Q.array[index].Weight {
			fmt.Printf("Child %d of Node %d is greater", 2*index+1, index)
			status = false
			break
		}
	}
	return status
}
