#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RECORDS    10000000
#define RECORD_LENGTH  100
#define KEY_LENGTH     10

struct helperStruct
{
	char key[KEY_LENGTH];
	char *actualData;
};

int myCompare(const void *a, const void *b)
{
	return (strcmp( ((struct helperStruct *) a)->key,((struct helperStruct *) b)->key));
}

int main(int argc, char **argv)
{
	char inputFileName[64] =  {'\0'};
	char outputFileName[64] = {'\0'};
	char **tempBuff;
	struct helperStruct *sortArray;
	FILE *fin,*fout;

	int i;
	struct timeval start_time,end_time;

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

	//TODO: check if there is any gain by grouping the NUM_RECORDS loops together.

	sortArray = (struct helperStruct *) malloc(sizeof(struct helperStruct) * NUM_RECORDS);
	tempBuff = (char **)malloc(sizeof(char *) * NUM_RECORDS);
	for (i = 0; i < NUM_RECORDS; i++)
	{
		tempBuff[i] = (char *)malloc(sizeof(char) * RECORD_LENGTH);
	}

	fin = fopen(inputFileName,"r");
	i = 0;

	while (fread(tempBuff[i],RECORD_LENGTH,1,fin) > 0)
	{
		strncpy(sortArray[i].key,tempBuff[i],KEY_LENGTH);
		sortArray[i].actualData = tempBuff[i];
		i++;
	}
	fclose(fin);
	qsort(sortArray,NUM_RECORDS,sizeof(struct helperStruct),myCompare);

	fout = fopen(outputFileName,"w");
	
	for (i = 0; i < NUM_RECORDS; i++)
	{
		fwrite(sortArray[i].actualData,RECORD_LENGTH,1,fout);
	}
	fclose(fout);
	for ( i = 0; i < NUM_RECORDS; i++)
	{
		free(tempBuff[i]);
	}
	free(tempBuff);
	gettimeofday(&end_time,NULL);
	printf("\nSorted %ld records in  %lf s\n",NUM_RECORDS,(end_time.tv_sec - start_time.tv_sec) +  (end_time.tv_usec - start_time.tv_usec)/1000000.0);

	return 0;
}
