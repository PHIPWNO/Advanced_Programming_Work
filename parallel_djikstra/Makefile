all: Main HeapTest NaiveCorrectnessTest SerialHeapCorrectnessTest Parallel

Main: correctness_testing.c heap.c heap.h main.c
	gcc -o Main -Wall main.c correctness_testing.c graph.c timer.c dijkstra.c heap.c

HeapTest: heap.c heap.h heap_test.c
	gcc -o HeapTest -Wall timer.c heap.c heap_test.c

NaiveCorrectnessTest: naive_correctness_testing.c correctness_testing.c heap.c heap.h
	gcc -o NaiveCorrectnessTest -Wall correctness_testing.c naive_correctness_testing.c graph.c timer.c dijkstra.c heap.c

SerialHeapCorrectnessTest: serial_min_correctness_testing.c correctness_testing.c heap.c heap.h
	gcc -o SerialHeapCorrectnessTest -Wall correctness_testing.c serial_min_correctness_testing.c graph.c timer.c dijkstra.c heap.c

Parallel: serial_min_correctness_testing.c correctness_testing.c heap.c heap.h parallel.c parallel_main.c 
	gcc -o Parallel -Xpreprocessor -fopenmp -Wall correctness_testing.c parallel.c graph.c timer.c dijkstra.c heap.c parallel_main.c -lomp