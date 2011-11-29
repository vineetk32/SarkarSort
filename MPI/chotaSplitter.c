
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECORD_LENGTH 100
#define FILE_SEEK_SIZE 100000
#define BUCKET_SIZE 100000
#define FILE_NAME 100

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
	FILE *handles[128][2];
	FILE *finput;
	int currChar;
	int nextChar;
	DataBucket buckets[128][2];
	int j, k;

	char tempFileName[FILE_NAME] = { '\0' };
	char ipFileName[FILE_NAME] = { '\0' };
	char *tempBuff;
	int i;
	int numRecords = FILE_SEEK_SIZE;

	int partition=0;

	for (i = 32; i < 127; i++) {
		for (j = 0; j < 2; j++)
			init_buckets(&buckets[i][j]);
	}
	tempBuff = (char *) malloc(sizeof(char) * FILE_SEEK_SIZE * RECORD_LENGTH);

	for (i = 32; i < 127; i++)
		for (j = 0; j < 2; j++) {
			sprintf(tempFileName, "%s_%d_%d0000000", argv[2], i, j);
			handles[i][j] = fopen(tempFileName, "wb");
		}

	j = 0;
	for (partition = 0; partition < 100; partition++) { //TODO;
		j = 0;
		sprintf(ipFileName, "%s_%d0000000", argv[1], partition);
		finput = fopen(ipFileName, "rb");
		do {
			numRecords = fread(tempBuff, RECORD_LENGTH, FILE_SEEK_SIZE, finput);
			//printf("%d\n",numRecords);
			for (i = 0; i < numRecords; i++) {
				currChar = tempBuff[i * RECORD_LENGTH];
				nextChar = tempBuff[i * RECORD_LENGTH + 1];

				strncpy(
						&buckets[currChar][nextChar < 79 ? 0 : 1].dataStore[RECORD_LENGTH
								* buckets[currChar][nextChar < 79 ? 0 : 1].numRecords],
						&tempBuff[i * RECORD_LENGTH], RECORD_LENGTH);
				buckets[currChar][nextChar < 79 ? 0 : 1].numRecords++;
			}
			j++;
			if (j > 40) {
			//	printf("writing");
				for (i = 32; i < 127; i++)
					for (k = 0; k < 2; k++)
						//printf("\n Bucket : %d Num of records %d", i,	buckets[i].numRecords);
						fwrite(buckets[i][k].dataStore, RECORD_LENGTH
								* buckets[i][k].numRecords, 1, handles[i][k]);
				j = 0;

				//reinit buffers
				for (i = 32; i < 127; i++)
					for (k = 0; k < 2; k++)
						buckets[i][k].numRecords = 0;
			}

		} while (numRecords == FILE_SEEK_SIZE);

		if (j <= 40) {

			//printf("writing1");
			for (i = 32; i < 127; i++)
				for (k = 0; k < 2; k++)
					//printf("\n Bucket : %d Num of records %d", i,	buckets[i].numRecords);
					fwrite(buckets[i][k].dataStore, RECORD_LENGTH
							* buckets[i][k].numRecords, 1, handles[i][k]);

			j = 0;

			for (i = 32; i < 127; i++)
				for (k = 0; k < 2; k++)
					buckets[i][k].numRecords = 0;
		}
		//delete file

		fclose(finput);
		printf("\n processed split %d",partition);
		//sleep(1);
		remove(ipFileName);
	}

	for (i = 32; i < 127; i++) {
		for (j = 0; j < 2; j++)
			if (handles[i][j] != NULL) {
				fclose(handles[i][j]);
			}
	}
	return 0;

}

