#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void* sequentialRead(void *);
void* sequentialReadWrite(void *);
void* randomRead(void *);
void runSequentialRead(int);
void runSequentialReadWrite(int);
void runRandomRead(int);
void createFile();

long int fileSize = 10737418240;  //10 GB of file size
struct timeval startTime, endTime;
long int blockSize = 0;
char* filename = "disk.txt";

int main(int argc, char *argv[]){

	int noOfThreads[4] = {1,2,4,8};
	int i, j;
	int blockSizes[4] = {8, (8*1024), (8*1024*1024), (80*1024*1024)};
	createFile(); //creates a file of size 10GB 
	size_t blockLength = sizeof(blockSizes)/sizeof(int);
	size_t threadLength = sizeof(noOfThreads)/sizeof(int);
	printf("block Length %d\n", blockLength);
	printf("thread Length %d\n", threadLength);
	for(i=0; i<threadLength; i++){
		for(j=0; j<blockLength; j++){
			blockSize = blockSizes[j];
			printf("block Size %d\n", blockSize);
			runSequentialRead(noOfThreads[i]);
			runSequentialReadWrite(noOfThreads[i]);
			runRandomRead(noOfThreads[i]);
		}
	}
	return 0;
}

void createFile()
{
	FILE *file=fopen(filename, "w");	//preform read and write operation which also creates file if not present
	fseek(file, fileSize-1, SEEK_SET);	//create file of 10GB starting from 1st byte
	fputc('v', file);	//put data into file
	fclose(file);	//close file
}

void runSequentialRead(int numOfThreads){

	int thread = 0;
	int *nt = numOfThreads;
	pthread_t thread_ID[numOfThreads];
	printf("Performing Disk Benchmarking for Sequential Read Operation using %d threads for %Ld block\n", numOfThreads, blockSize);
	gettimeofday(&startTime, NULL);
	double strTime = startTime.tv_sec + (startTime.tv_usec/1000000.0);	
	
	for(thread=0;thread<numOfThreads;thread++){
		pthread_create(&thread_ID[thread],NULL,sequentialRead,(void *)&nt);
	}
	
	for(thread=0;thread<numOfThreads;thread++){
		pthread_join(thread_ID[thread],NULL);
	}
	gettimeofday(&endTime, NULL);
	double edTime = endTime.tv_sec+(endTime.tv_usec/1000000.0);
	double ttlTime = edTime - strTime;
	printf("Total Time taken: %lf\n",ttlTime);
	printf("Throughput is: %lf MBPS\n",(fileSize/ttlTime)/(1024*1024));
    printf("Latency is: %0.12lf ms\n",(ttlTime*1000)/(fileSize/blockSize));
}

void runRandomRead(int numOfThreads){
	int thread = 0;
	int *nt = numOfThreads;
	pthread_t thread_ID[numOfThreads];
	printf("Performing Disk Benchmarking for Random Read Operation using %d threads for %Ld block\n", numOfThreads, blockSize);
	gettimeofday(&startTime, NULL);
	double strTime = startTime.tv_sec + (startTime.tv_usec/1000000.0);	
	
	for(thread=0;thread<numOfThreads;thread++){
		pthread_create(&thread_ID[thread],NULL,randomRead,(void *)&nt);		
	}

	for(thread=0;thread<numOfThreads;thread++){
		pthread_join(thread_ID[thread],NULL);
	}
	
	gettimeofday(&endTime, NULL);
	double edTime = endTime.tv_sec+(endTime.tv_usec/1000000.0);
	double ttlTime = edTime - strTime;
	
	printf("Total Time taken: %lf\n",ttlTime);
    printf("Throughput is: %lf MBPS\n",(fileSize/ttlTime)/(1024*1024));
    printf("Latency is: %0.12lf ms\n",(ttlTime*1000)/(fileSize/blockSize));
}

void runSequentialReadWrite(int numOfThreads){

	int thread = 0;
	int *nt = numOfThreads;
	pthread_t thread_ID[numOfThreads];
	printf("Performing Disk Benchmarking for Sequential Read Write Operation using %d threads for %Ld block\n", numOfThreads, blockSize);
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
    printf("Throughput is: %lf MBPS\n",(fileSize/ttlTime)/(1024*1024));
    printf("Latency is: %0.12lf ms\n",(ttlTime*1000)/(fileSize/blockSize));
}

void *sequentialReadWrite(void *numOfThreads){
	srand((unsigned)time(NULL));
	int noOfThreads = *((int *)numOfThreads);
	int filedesc = open(filename, O_RDONLY, 0666);	//open file in read only mode
	long int bSize = blockSize;
	int blockFile = fileSize/bSize;
	int blockFilePerThread = blockFile/noOfThreads;
	
	int i, temp=0;
	
	char *seqReadWriteBlock = (char*)malloc(bSize*sizeof(char));
	for(i=0; i<blockFilePerThread; i++){
		temp+=bSize;
		lseek(filedesc, temp, SEEK_SET); 
		read(filedesc, seqReadWriteBlock, bSize);	
		write(filedesc, seqReadWriteBlock, bSize);	
	}
	free(seqReadWriteBlock);
	pthread_exit(NULL);
}

void *sequentialRead(void *numOfThreads){
	srand((unsigned)time(NULL));
	int noOfThreads = *((int *)numOfThreads);
	long int bSize = blockSize;
	int blockFile = fileSize/bSize;
	int blockFilePerThread = blockFile/noOfThreads;
	int i, temp=0;
	int filedesc = open(filename, O_RDONLY, 0666);	//open file in read only mode
	char *seqReadBlock = (char*)malloc(bSize*sizeof(char));
	
	for(i=0; i<blockFilePerThread; i++){
		temp+=bSize;
		lseek(filedesc, temp, SEEK_SET);
		read(filedesc, seqReadBlock, bSize);
	}
	
	free(seqReadBlock);
	pthread_exit(NULL);
}

void *randomRead(void *numOfThreads){

	srand((unsigned)time(NULL));
	
	int noOfThreads = *((int *)numOfThreads);
	
	long int bSize = blockSize;
	int blockFile = fileSize/bSize;
	int blockFilePerThread = blockFile/noOfThreads;
	
	int i, randomNumber=0;
	int filedesc = open(filename, O_RDONLY, 0666);	//open file in read only mode
	if(filedesc < 0)
	{
		printf("Open test file failed!\n");
		exit(0);
	}	
	//char *randomWrite1 = (char*)malloc(fileSize*sizeof(char));
	char *randomReadBlock = (char*)malloc(bSize*sizeof(char));
	for(i=0; i<blockFilePerThread; i++){
		randomNumber = rand()% blockFilePerThread;
		lseek(filedesc, randomNumber+bSize, SEEK_SET);
		read(filedesc, randomReadBlock, bSize);
	}
	
	free(randomReadBlock);
	pthread_exit(NULL);
}