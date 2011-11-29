#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECORD_LENGTH  100
#define KEY_LENGTH     10
#define FILE_NAME 100

#define NUM_REC_IN_BUC 3000
#define BUFFER_SIZE 1000
#define TEMP_BUFFER_SIZE 1000
#define DATA_STORE_SIZE 6000000

typedef struct helperStruct {
	char key[KEY_LENGTH];
	char *actualData;
} Record;

typedef struct bucket {
	long numRecords;
	Record *records;
} Bucket;

int myCompare(const void *a, const void *b) {
	return (strcmp(((struct helperStruct *) a)->key + 1,
			((struct helperStruct *) b)->key + 1));
}

int main(int argc, char * argv[]) {

	char *dataStore = NULL;
	char *tempbuff;
	char *buffer;
	FILE *intmfile, *finalfile;
	char intmFileName[FILE_NAME] = { '\0' };
	char finalFileName[FILE_NAME] = { '\0' };
	int fileCounter, bucket;
	long fileLength;
	long numRecords;
	int dataStoreLocn;
	int i, j, k;
	Record *tempRecord;
	Bucket bucketPtr;
	char pos2, pos3;

	Bucket buckets[128][128];

	for (i = 32; i < 127; i++) {
		for (j = 32; j < 127; j++) {
			buckets[i][j].numRecords = 0;
			buckets[i][j].records = (Record *) malloc(sizeof(Record)
					* NUM_REC_IN_BUC);
		}
	}

	//tempbuff = (char *) malloc(sizeof(char) * RECORD_LENGTH * TEMP_BUFFER_SIZE);
	dataStore = (char *) malloc(sizeof(char) * DATA_STORE_SIZE * RECORD_LENGTH);
	for (fileCounter = 32; fileCounter < 127; fileCounter++) {
		for (bucket = 0; bucket < 2; bucket++) {
			//read data
			sprintf(intmFileName, "%s_%d_%d0000000", argv[1], fileCounter, bucket);
			intmfile = fopen(intmFileName, "rb");

			for (i = 32; i < 127; i++) {
					for (j = 32; j < 127; j++) {
						buckets[i][j].numRecords = 0;
					}
			}

			//may cause prob
			dataStoreLocn = 0;
			do {
				buffer = &dataStore[dataStoreLocn*RECORD_LENGTH];
				numRecords
						= fread(buffer, RECORD_LENGTH, BUFFER_SIZE, intmfile);
				for (i = 0; i < numRecords; i++) {
					pos2 = buffer[i*RECORD_LENGTH + 1];
					pos3 = buffer[i*RECORD_LENGTH + 2];
					tempRecord
							= &buckets[pos2][pos3].records[buckets[pos2][pos3].numRecords++];
					tempRecord->actualData = &buffer[i*RECORD_LENGTH];
					strncpy(tempRecord->key, &buffer[i*RECORD_LENGTH], KEY_LENGTH);
				}
				dataStoreLocn += numRecords;

			} while (numRecords == BUFFER_SIZE);

			fclose(intmfile);

			for (i = 32; i < 127; i++) {
				for (j = 32; j < 127; j++) {
					qsort(buckets[i][j].records, buckets[i][j].numRecords,
							sizeof(struct helperStruct), myCompare);
				}

			}
			sprintf(finalFileName, "%s_%d_%d", argv[2], fileCounter,bucket);
			finalfile = fopen(finalFileName, "wb");
			for (i = 32; i < 127; i++) {
				for (j = 32; j < 127; j++) {
					bucketPtr = buckets[i][j];
					for (k = 0; k < bucketPtr.numRecords; k++)
						fwrite(bucketPtr.records[k].actualData, 1,
								RECORD_LENGTH, finalfile); //TODO need to amortize this
				}
			}
			fclose(finalfile);
			remove(intmFileName);
			printf("\nprocessed file : %s",intmFileName);
		}

	}
	return 0;

}
