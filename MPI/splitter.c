#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define RECORD_LENGTH 100
#define FILE_SEEK_SIZE 100000
#define BUCKET_SIZE 100000

typedef struct dataBucket {
	char *dataStore;
	int numRecords;
} DataBucket;

void init_buckets(DataBucket *bucket) {
	bucket->dataStore = (char *) malloc(sizeof(char) * RECORD_LENGTH
			* BUCKET_SIZE);
	bucket->numRecords = 0;
}

int main(int argc, char **argv) {
	FILE *handles[128];
	FILE *finput;
	int currChar;
	DataBucket buckets[128];
	int j;

	char tempFileName[32] = { '\0' };
	char *tempBuff;
	int i;
	int numRecords = FILE_SEEK_SIZE;
	struct timeval start_time,end_time;
	gettimeofday(&start_time,NULL);
	for (i = 32; i < 127; i++) {
		init_buckets(&buckets[i]);
	}
	tempBuff = (char *) malloc(sizeof(char) * FILE_SEEK_SIZE * RECORD_LENGTH);
	finput = fopen(argv[1], "rb");

	for (i = 32; i < 127; i++) {
		sprintf(tempFileName, "%s.%d", argv[1], i);
		handles[i] = fopen(tempFileName, "wb");
	}

	j = 0;
	do {
		numRecords = fread(tempBuff, RECORD_LENGTH, FILE_SEEK_SIZE, finput);
		//printf("%d\n",numRecords);
		for (i = 0; i < numRecords; i++) {
			currChar = tempBuff[i * RECORD_LENGTH];

			strncpy(&buckets[currChar].dataStore[RECORD_LENGTH * buckets[currChar].numRecords], &tempBuff[i * RECORD_LENGTH], RECORD_LENGTH);
			buckets[currChar].numRecords++;
		}
		j++;
		if (j > 90) {
			for (i = 32; i < 127; i++)
				//printf("\n Bucket : %d Num of records %d", i,buckets[i].numRecords);
			fwrite(buckets[i].dataStore, RECORD_LENGTH * buckets[i].numRecords, 1, handles[i]);
			j = 0;

			//reinit buffers
			for (i = 32; i < 127; i++)
				buckets[i].numRecords = 0;
		}

	} while (numRecords == FILE_SEEK_SIZE);

	if (j <= 90) {
		for (i = 32; i < 127; i++)
			fwrite(buckets[i].dataStore, RECORD_LENGTH * buckets[i].numRecords,
					1, handles[i]);
	}
	fclose(finput);
	for (i = 32; i < 127; i++) {
		if (handles[i] != NULL) {
			fclose(handles[i]);
		}
	}
	gettimeofday(&end_time,NULL);
	printf("\nTotal execution time - %lf s\n",(end_time.tv_sec - start_time.tv_sec) +  (end_time.tv_usec - start_time.tv_usec)/1000000.0);
	return 0;
}
