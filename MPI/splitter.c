#include <stdio.h>
#define RECORD_LENGTH 100
int main(int argc,char **argv)
{
	FILE *handles[256];
	FILE *finput;
	int currChar;
	//char fileNamePrefix[] = {'f','i','l','e','c','h','u','n','k','\0'};
	char tempFileName[32] = {'\0'};
	char tempBuff[100] = {'\0'};
	int i;

	finput = fopen(argv[1],"rb");
	for (i = 0; i < 255; i++)
	{
		handles[i] = NULL;
	}
	while (fread(tempBuff,100,1,finput) > 0 )
	{
		//printf("\nDEBUG: offset - %ld currChar - %d",offset,currChar);
		currChar = tempBuff[0];
		if (handles[currChar] == NULL)
		{
			sprintf(tempFileName,"%s.%d",argv[1],currChar);
			handles[currChar] = fopen(tempFileName,"wb");
		}
		fwrite(tempBuff,100,1,handles[currChar]);
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
