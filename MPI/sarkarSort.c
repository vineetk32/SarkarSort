#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_RECORDS    10000000
#define RECORD_LENGTH  100
#define KEY_LENGTH     10
#define THREAD_MAX     8

#define MAX_RECORDS_PER_CHUNK  NUM_RECORDS / 10
#define MAX_CHUNK_SIZE 255

struct helperStruct
{
	char key[KEY_LENGTH];
	char *actualData;
};

struct threadPackage
{
	char *inputFilePrefix;
	char *outputFilePrefix;
	int *commandArray;
};


inline int myCompare(const void *a, const void *b)
{
	return (strcmp( ((struct helperStruct *) a)->key,((struct helperStruct *) b)->key));
}

void *threadFunc(void *args)
{
	//Each thread will get a range of characters to work on.
	int i,counter = 0,j;
	char chunkFileName[64] =  {'\0'};
	char outputFileName[64] = {'\0'};
	FILE *fin,*fout;
	int numRecords = MAX_RECORDS_PER_CHUNK;
	char **tempBuff;
	struct helperStruct *sortArray;

	struct threadPackage * package = (struct threadPackage *) args;

	sortArray = (struct helperStruct *) malloc(sizeof(struct helperStruct) * numRecords);
	tempBuff = (char **)malloc(sizeof(char *) * numRecords);
	for (i = 0; i < numRecords; i++)
	{
		tempBuff[i] = (char *)malloc(sizeof(char) * RECORD_LENGTH);
	}
	for (i = 0; package->commandArray[i] != -1; i++)
	{
		sprintf(chunkFileName,"%s.%d",package->inputFilePrefix,package->commandArray[i]);
		sprintf(outputFileName,"%s.sorted.%d",package->outputFilePrefix,package->commandArray[i]);
		
		fin = fopen(chunkFileName,"r");
		if (fin == NULL)
		{
			//printf("\n Couldnt open %s - ignoring",chunkFileName);
			//pthread_exit(NULL);
			continue;
		}

		//printf("\n Opened %s - sorting",chunkFileName);
		counter = 0;
		while (!feof(fin))
		{
			tempBuff[counter][0] = '\0';
			if (fread(tempBuff[counter],RECORD_LENGTH,1,fin) > 0)
			{			
				sortArray[counter].key[0] = '\0';
				sortArray[counter].actualData = NULL;
				strncpy(sortArray[counter].key,tempBuff[counter],KEY_LENGTH);
				sortArray[counter].actualData = tempBuff[counter];
				counter++;
			}
		}
		fclose(fin);
		qsort(sortArray,counter,sizeof(struct helperStruct),myCompare);
		fout = fopen(outputFileName,"w");
		
		for (j = 0; j < counter; j++)
		{
			fwrite(sortArray[j].actualData,RECORD_LENGTH,1,fout);
		}
		fclose(fout);
	}	
	for (i = 0; i < numRecords; i++)
	{
		free(tempBuff[i]);
	}
	free(tempBuff);
	return;
}

int main(int argc, char **argv)
{
	struct timeval start_time,end_time;
	pthread_t threads[THREAD_MAX];
	struct threadPackage packages[THREAD_MAX];
	int numThreads;

	char inputFileName[64] =  {'\0'};
	char outputFileName[64] = {'\0'};
	int i,j = 0,idx;

	gettimeofday(&start_time,NULL);

	//TODO: Add validation.
	if (argc == 4)
	{
		strcpy(inputFileName,argv[1]);
		strcpy(outputFileName,argv[2]);
		numThreads = atoi(argv[3]);
	}
	else
	{
		printf("\nUsage - %s <inputFileName> <outputFilePrefix> <numThreads>\n",argv[0]);
		return 0;
	}

	//Spawn threads here and sort individual files.
	
	for (i = 0; i < numThreads; i++)
	{
		packages[i].inputFilePrefix = inputFileName;
		packages[i].outputFilePrefix = outputFileName;
		packages[i].commandArray = (int *) malloc (sizeof(int) * (NUM_RECORDS / numThreads + 1));

		for (idx = 0;j < MAX_CHUNK_SIZE;j++,idx++)
		{
			if (idx >=  (MAX_CHUNK_SIZE) / numThreads)
			{
				break;
			}
			//populate package->commandArray here.
			packages[i].commandArray[idx] = j;
		}
		packages[i].commandArray[idx] = -1;
		if (pthread_create(&threads[i],NULL,threadFunc,(void *)&packages[i]) != 0)
		{
			printf("\nError creating thread %d\n",i);
		}
	}
	//Maybe something useful can be done here?
	
	for (i = 0; i < numThreads; i++)
	{
		pthread_join(threads[i],NULL);
	}
	gettimeofday(&end_time,NULL);
	printf("\nTotal execution time - %lf s\n",(end_time.tv_sec - start_time.tv_sec) +  (end_time.tv_usec - start_time.tv_usec)/1000000.0);



	return 0;
}
