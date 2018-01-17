#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void* sequentialWrite(void *);
void* sequentialReadWrite(void *);
void* randomWrite(void *);
void runSequentialWrite(int);
void runSequentialReadWrite(int);
void runRandomWrite(int);

long int memorySpace = 1374389535;  //1.28 GB of memory space
struct timeval startTime, endTime;
long int blockSize = 0;

int main(int argc, char *argv[]){

	int noOfThreads[4] = {1,2,4,8};
	int i, j;
	int blockSizes[4] = {8, (8*1024), (8*1024*1024), (80*1024*1024)};
	size_t blockLength = sizeof(blockSizes)/sizeof(int);
	size_t threadLength = sizeof(noOfThreads)/sizeof(int);
	printf("block Length %d\n", blockLength);
	printf("thread Length %d\n", threadLength);
	for(i=0; i<threadLength; i++){
		for(j=0; j<blockLength; j++){
			blockSize = blockSizes[j];
			printf("block Size %d\n", blockSize);
			runSequentialWrite(noOfThreads[i]);
			runSequentialReadWrite(noOfThreads[i]);
			runRandomWrite(noOfThreads[i]);
		}
	}
	return 0;
}

void runSequentialWrite(int numOfThreads){

	int thread = 0;
	int *nt = numOfThreads;
	pthread_t thread_ID[numOfThreads];
	printf("Performing Memory Benchmarking for Sequential Write Operation using %d threads for %Ld block\n", numOfThreads, blockSize);
	gettimeofday(&startTime, NULL);
	double strTime = startTime.tv_sec + (startTime.tv_usec/1000000.0);	
	
	for(thread=0;thread<numOfThreads;thread++){
		pthread_create(&thread_ID[thread],NULL,sequentialWrite,(void *)&nt);
	}
	

	for(thread=0;thread<numOfThreads;thread++){
		pthread_join(thread_ID[thread],NULL);
	}
	gettimeofday(&endTime, NULL);
	double edTime = endTime.tv_sec+(endTime.tv_usec/1000000.0);
	double ttlTime = edTime - strTime;
	printf("Total Time taken: %lf\n",ttlTime);
    printf("Throughput is: %lf MBPS\n",(memorySpace/ttlTime)/(1024*1024));
    printf("Latency is: %0.12lf ms\n",(ttlTime*1000)/(memorySpace/blockSize));
}

void runRandomWrite(int numOfThreads){
	int thread = 0;
	int *nt = numOfThreads;
	pthread_t thread_ID[numOfThreads];
	printf("Performing Memory Benchmarking for Random Write Operation using %d threads for %Ld block\n", numOfThreads, blockSize);
	gettimeofday(&startTime, NULL);
	double strTime = startTime.tv_sec + (startTime.tv_usec/1000000.0);	
	
	for(thread=0;thread<numOfThreads;thread++){
		pthread_create(&thread_ID[thread],NULL,randomWrite,(void *)&nt);		
	}


	for(thread=0;thread<numOfThreads;thread++){
		pthread_join(thread_ID[thread],NULL);
	}
	
	gettimeofday(&endTime, NULL);
	double edTime = endTime.tv_sec+(endTime.tv_usec/1000000.0);
	double ttlTime = edTime - strTime;
	
	printf("Total Time taken: %lf\n",ttlTime);
	printf("Throughput is: %lf MBPS\n",(memorySpace/ttlTime)/(1024*1024));
    printf("Latency is: %0.12lf ms\n",(ttlTime*1000)/(memorySpace/blockSize));
}

void runSequentialReadWrite(int numOfThreads){

	int thread = 0;
	int *nt = numOfThreads;
	pthread_t thread_ID[numOfThreads];
	printf("Performing Memory Benchmarking for Sequential Read Write Operation using %d threads for %Ld block\n", numOfThreads, blockSize);
	gettimeofday(&startTime, NULL);
	double strTime = startTime.tv_sec + (startTime.tv_usec/1000000.0);	
	
	for(thread=0;thread<numOfThreads;thread++){
		pthread_create(&thread_ID[thread],NULL,sequentialReadWrite,(void *)&nt);
	}

	for(thread=0;thread<numOfThreads;thread++){
		pthread_join(thread_ID[thread],NULL);
	}
	
	gettimeofday(&endTime, NULL);
	double edTime = endTime.tv_sec+(endTime.tv_usec/1000000.0);
	double ttlTime = edTime - strTime;
	
	printf("Total Time taken: %lf\n",ttlTime);
    printf("Throughput is: %lf MBPS\n",(memorySpace/ttlTime)/(1024*1024));
    printf("Latency is: %0.12lf ms\n",(ttlTime*1000)/(memorySpace/blockSize));
}

void *sequentialReadWrite(void *numOfThreads){
	srand((unsigned)time(NULL));
	int noOfThreads = *((int *)numOfThreads);
	
	long int bSize = blockSize;
	int blockMemory = memorySpace/bSize;
	int blockMemoryPerThread = blockMemory/noOfThreads;
	
	int i, temp=0;
	
	char *seqRead1 = (char*)malloc(memorySpace*sizeof(char));
	char *seqRead2 = (char*)malloc(bSize*sizeof(char));
	
	for(i=0; i<blockMemoryPerThread; i++){
		memcpy(seqRead2, seqRead1+temp, bSize);
		temp+=bSize;
	}
	
	free(seqRead1);
	free(seqRead2);
	pthread_exit(NULL);
}

void *sequentialWrite(void *numOfThreads){
	srand((unsigned)time(NULL));
	int noOfThreads = *((int *)numOfThreads);
	long int bSize = blockSize;
	int blockMemory = memorySpace/bSize;
	int blockMemoryPerThread = blockMemory/noOfThreads;
	int i, temp=0;
	
	char *seqWrite1 = (char*)malloc(memorySpace*sizeof(char));
	
	for(i=0; i<blockMemoryPerThread; i++){
		memset(seqWrite1+temp, 'v', bSize);
		temp+=bSize;
	}
	
	free(seqWrite1);
	pthread_exit(NULL);
}

void *randomWrite(void *numOfThreads){

	srand((unsigned)time(NULL));
	
	int noOfThreads = *((int *)numOfThreads);
	
	long int bSize = blockSize;
	int blockMemory = memorySpace/bSize;
	int blockMemoryPerThread = blockMemory/noOfThreads;
	
	int i, randomNumber=0;
	
	char *randomWrite1 = (char*)malloc(memorySpace*sizeof(char));
	
	for(i=0; i<blockMemoryPerThread; i++){
		randomNumber = rand()% blockMemoryPerThread;
		memset(randomWrite1+randomNumber, 'v', bSize);
	}
	
	free(randomWrite1);
	pthread_exit(NULL);
}