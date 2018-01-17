#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

struct timeval startTime,endTime;
int numOfLoops = 1000000000;

void *intergerCPUBenchMarking(void *);
void runIntegerThreads(int);
void *floatCPUBenchMarking(void *);
void runFloatingThread(int);

void main(int argc, char *argv[])
{
	int choice[2] = {1,2};			//array of choice for integer and float operation. 1 for integer and 2 for float
	int noOfThreads[4] = {1,2,4,8};	//array of threads
	
	int i, j;
	size_t choiceLength = sizeof(choice)/sizeof(int);		//choice array size
	size_t threadLength = sizeof(noOfThreads)/sizeof(int);	//noOfThreads array size
	for(i=0; i<choiceLength; i++){
		for(j =0; j<threadLength; j++){
			if(choice[i] == 1){
				runIntegerThreads(noOfThreads[j]);
			}
			if(choice[i] == 2){
				runFloatingThread(noOfThreads[j]);
			}
		}
	}
}

void runIntegerThreads(int numOfThreads)
{
	int thread = 0;
	int *nt = numOfThreads;
	pthread_t *thread_ID = malloc(sizeof(pthread_t)*numOfThreads);
	printf("Integer Benchmarking of %d threads\n",numOfThreads);
	gettimeofday(&startTime, NULL);
	double strTime = startTime.tv_sec+(startTime.tv_usec/1000000.0);

	for(thread=0;thread<numOfThreads;thread++){
		pthread_create(&thread_ID[thread],NULL,intergerCPUBenchMarking,(void *)&nt);
	}

	for(thread=0;thread<numOfThreads;thread++){
		pthread_join(thread_ID[thread],NULL);
	}
		
	gettimeofday(&endTime, NULL);
	double edTime = endTime.tv_sec+(endTime.tv_usec/1000000.0);
	double ttlTime = edTime - strTime;
	printf("Integer Operation @ %lf GIOPS\n",(40/(ttlTime)));
}

void *intergerCPUBenchMarking(void *numOfThreads)
{
	int n1 = 7, n2 = 4, n3 = 35, numOfInstruction = 8, nt;
	nt = *((int *)numOfThreads);
	int i,j;

	for (i = 0; i < numOfLoops; i++) {
		//total 40 operations
		for(j = 0; j< numOfInstruction/nt; j++){
			n3/n1-n2;
		}
	}
	
	pthread_exit(NULL);
}

void runFloatingThread(int numOfThreads)
{
	int thread=0;
	int *nt = numOfThreads;
	pthread_t *thread_ID = malloc(sizeof(pthread_t)*numOfThreads);

	printf("Floating point Benchmarking of %d threads\n",numOfThreads);

	gettimeofday(&startTime, NULL);
	double strTime = startTime.tv_sec+(startTime.tv_usec/1000000.0);

	for(thread=0;thread<numOfThreads;thread++){
		pthread_create(&thread_ID[thread],NULL,floatCPUBenchMarking,(void *)&nt);
	}
	for(thread=0;thread<numOfThreads;thread++){
		pthread_join(thread_ID[thread],NULL);
	}
	
	gettimeofday(&endTime, NULL);
	double edTime = endTime.tv_sec+(endTime.tv_usec/1000000.0);
	double ttlTime = edTime - strTime;

	printf("Floating Point Operation @ %lf GFLOPS\n",(40/(ttlTime)));
}


void *floatCPUBenchMarking(void *numOfThreads)
{
	int numOfInstruction = 8, nt;
	nt = *((int *)numOfThreads);
	int i, j;
	float n1 = 6.24f, n2 = 9.87f, n3 = 101.34f;

	// total 40 operations 
	for (i = 0; i < numOfLoops; i++) {
		for(j = 0; j< numOfInstruction/nt; j++){
			n3/n1-n2;
		}
	}
	pthread_exit(NULL);
}