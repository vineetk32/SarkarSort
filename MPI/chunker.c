#include <stdio.h>
#define RECORD_LENGTH 100
int main()
{
	FILE *handles[256];
	FILE *finput;
	long int offset = 0;
	int currChar;
	char fileNamePrefix[] = {'c','h','u','n','k','\0'};
	char tempFileName[32] = {'\0'};
	int i;

	finput = fopen("input.txt","r");
	for (i = 0; i < 255; i++)
	{
	//	sprintf(tempFileName,"%s_%d",fileNamePrefix,i);
		handles[i] = NULL;
	}
	//for (i = 0; i < 255; i++)
	//{
	//	sprintf(tempFileName,"%s_%d",fileNamePrefix,i);
	//	handles[i] = fopen(tempFileName,"w");
	
	//}
	while ((currChar = fgetc(finput)) != EOF)
	{
		//printf("\nDEBUG: offset - %ld currChar - %d",offset,currChar);
		if (handles[currChar] == NULL)
		{

			sprintf(tempFileName,"%s_%d",fileNamePrefix,currChar);
			handles[currChar] = fopen(tempFileName,"w");
		}
		fprintf(handles[currChar],"%ld\n",offset);
		offset += RECORD_LENGTH;
		fseek(finput,offset,SEEK_SET);
	}
	for (i = 0; i < 255; i++)
	{
		if (handles[i] != NULL)
		{
			fclose(handles[i]);
		}
	}
	return 0;
}
