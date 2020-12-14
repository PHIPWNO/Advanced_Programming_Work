#include<stdlib.h>
#include<stdio.h>
#include"timer.h"

/* just to test openMP stuff */
int main(int argc, char **argv){
	/*
	char *inputs[4] = {"graph4.txt", "graph3.txt", "graph1.txt",
	"graph2.txt"};*/
	int loop_size = 5000; double OMPtime, Serialtime, sum = 0;

		

	/* can choose to do big file or just small ones */ 
	
	if(argc > 1){
		loop_size = atoi(argv[1]);
	}
	printf("Testing summing over loop in openMP for %d iterations \n", loop_size);
	StartTimer();
#pragma omp parallel default(none) shared(loop_size) num_threads(8)
{
#pragma omp for schedule(dynamic)
	for (int i = 0; i < loop_size; ++i)
	{
		sum += i;
		//printf("printing %d\n", i);
	}
}	
	
	printf("sum is %lf\n", sum);
	sum = 0;

	OMPtime = GetTimer() / 1000.0;
	printf("COMPLETE IN OPENMP TIME:%lf\n", OMPtime);

	StartTimer();

	for (int i = 0; i < loop_size; ++i)
	{
		sum += i;
		//printf("printing %d\n", i);
	
	}
	
	Serialtime = GetTimer() / 1000.0;
	

	printf("COMPLETE IN SERIAL TIME:%lf\n", Serialtime);
	printf("sum is %lf\n", sum);
	

	printf("OpenMP Time:%lf \nSerial Time:%lf\n", OMPtime, Serialtime);
	return 0;
	
}


