#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_RECORDS    10000000
#define RECORD_LENGTH  100
#define KEY_LENGTH     10
#define THREAD_MAX     8

#define MAX_RECORD_PER_CHUCK  NUM_RECORDS / 10

struct helperStruct
{
	char key[KEY_LENGTH];
	char *actualData;
};

struct threadPackage
{
	char *inputFileName;
	int *commandArray;
};


inline int myCompare(const void *a, const void *b)
{
	return (strcmp( ((struct helperStruct *) a)->key,((struct helperStruct *) b)->key));
}

void *threadFunc(void *args)
{
	//Each thread will get a range of characters to work on.
	int i;
	char chunkFileName[64] =  {'\0'};
	char outputFileName[64] = {'\0'};
	FILE *fhelp,*fout,*fmain;
	int numRecords = MAX_RECORDS_PER_CHUNK;
	char **tempBuff;
	struct helperStruct *sortArray;
	long int tempOffset;


	struct threadPackage package = (struct threadPackage *) args;

	sortArray = (struct helperStruct *) malloc(sizeof(struct helperStruct) * numRecords);
	tempBuff = (char **)malloc(sizeof(char *) * numRecords);
	for (i = 0; i < numRecords; i++)
	{
		tempBuff[i] = (char *)malloc(sizeof(char) * RECORD_LENGTH);
	}
	for (i = 0; package->commandArray[i] != -1; i++)
	{
		sprintf(chunkFileName,"%s.%d",i);
		sprintf(outputFileName,"chunk_%d_sorted",i);
		
		fin = fopen(helperFileName,"r");
		if (fin == NULL)
		{
			//pthread_exit()
		}


		while (!feof(fin))
		{
			tempBuff[0] = '\0';
			if (fread(tempBuff[i],RECORD_LENGTH,1,fin) > 0)
			{			
				sortArray[i].key[0] = '\0';
				sortArray[i].actualData = NULL;
				strncpy(sortArray[i].key,tempBuff[i],KEY_LENGTH);
				sortArray[i].actualData = tempBuff[i];
				i++;
			}
		}
		fclose(fin);
		qsort(sortArray,numRecords,sizeof(struct helperStruct),myCompare);
		fout = fopen(outputFileName,"w");
		
		for (i = 0; i < NUM_RECORDS; i++)
		{
			fwrite(sortArray[i].actualData,RECORD_LENGTH,1,fout);
		}
		fclose(fout);
	}	
	free(tempBuff);
}

int main(int argc, char **argv)
{
	FILE *fin,*fout;
	struct timeval start_time,end_time;
	pthread_t threads[THREAD_MAX];

	int i;

	gettimeofday(&start_time,NULL);

	//TODO: Add validation.
	if (argc == 3)
	{
		strcpy(inputFileName,argv[1]);
		strcpy(outputFileName,argv[2]);
	}
	else
	{
		printf("\nusage. %s <inputFileName> <outputFileName>\n",argv[0]);
		return 0;
	}

	//Spawn threads here and spling individual files.
	
	gettimeofday(&end_time,NULL);
	printf("\nTotal execution time - %lf s\n",(end_time.tv_sec - start_time.tv_sec) +  (end_time.tv_usec - start_time.tv_usec)/1000000.0);


	return 0;
}
