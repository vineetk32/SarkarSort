#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RECORDS    10000000
#define RECORD_LENGTH  100
#define KEY_LENGTH     10
#define BUCKET_SIZE    500000			//TODO replace with total records / num of buckets



typedef struct helperStruct {
	char key[KEY_LENGTH];
	char *actualData;
} Record;

typedef struct bucket {
	long numRecords;
	Record *records;
} Bucket;

void initBucket(Bucket *bucket) {
	bucket->numRecords = 0;
	bucket->records = (Record*) malloc(BUCKET_SIZE * sizeof(Record));
}

int myCompare(const void *a, const void *b) {
	return (strcmp(((struct helperStruct *) a)->key,
			((struct helperStruct *) b)->key));
}

void sortBucket(Bucket *bucket) {
	qsort(bucket->records, bucket->numRecords, sizeof(Record), myCompare);
}


int main(int argc, char **argv) {
	char inputFileName[64] = { '\0' };
	char outputFileName[64] = { '\0' };
	char **tempBuff;

	FILE *fin, *fout;

	Bucket buckets[256];
	int bucketLoop = 0;
	Record tempRecord;
	char splitChar;
	int j;

	long i;
	struct timeval start_time, end_time;

	gettimeofday(&start_time, NULL);

	//TODO: Add validation.
	if (argc == 3) {
		strcpy(inputFileName, argv[1]);
		strcpy(outputFileName, argv[2]);
	} else {
		printf("\nusage. %s <inputFileName> <outputFileName>\n", argv[0]);
		return 0;
	}

	//TODO: check if there is any gain by grouping the NUM_RECORDS loops together.


	tempBuff = (char **) malloc(sizeof(char *) * NUM_RECORDS);
	for (i = 0; i < NUM_RECORDS; i++) {
		tempBuff[i] = (char *) malloc(sizeof(char) * RECORD_LENGTH);
	}

	fin = fopen(inputFileName, "r");
	i = 0;

	for (bucketLoop = 0; bucketLoop < 256; bucketLoop++)
	{
		buckets[bucketLoop]->numRecords = 0;
		buckets[bucketLoop]->records = (Record*) malloc(BUCKET_SIZE * sizeof(Record));
	}

	//write to buckets

	while (fread(tempBuff[i], RECORD_LENGTH, 1, fin) > 0) {
		strncpy(tempRecord.key, tempBuff[i], KEY_LENGTH);
		tempRecord.actualData = tempBuff[i];
		splitChar = tempRecord.key[0];
		buckets[splitChar].records[buckets[splitChar].numRecords++] = tempRecord;
		i++;
	}

	//sort the buckets
	for (bucketLoop = 0; bucketLoop < 256; bucketLoop++)
		sortBucket(&buckets[bucketLoop]);

	fclose(fin);
	//qsort(sortArray, NUM_RECORDS, sizeof(struct helperStruct), myCompare);

	fout = fopen(outputFileName, "w");

	for (i = 0; i < 256; i++) {
	Bucket bucket = buckets[i];
		for(j=0;j<bucket.numRecords;j++)
			fwrite(bucket.records[j].actualData,1,RECORD_LENGTH,fout);
	}

	fclose(fout);
	for (i = 0; i < NUM_RECORDS; i++) {
		free(tempBuff[i]);
	}
	free(tempBuff);
	//TODO free buckets


	gettimeofday(&end_time, NULL);
	printf("\nSorted %ld records in  %lf s\n", NUM_RECORDS, (end_time.tv_sec
			- start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec)
			/ 1000000.0);

	return 0;
}
