#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

int fileLen(FILE *fp);
void fileCopy(FILE *fromStream, FILE *toStream, int fileLen);

int main()
{
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;
	
	char fileName[] = "D:\\Users\\PC81\\Desktop\\test.jpg";
	int len;

	fp1 = fopen(fileName, "rb");
	fp2 = fopen("D:\\Users\\PC81\\Desktop\\test2.jpg", "wb");

	len = fileLen(fp1);

	fileCopy(fp1, fp2, len);

	fclose(fp1);
	fclose(fp2);
	
	return 0;
}

int fileLen(FILE *fp)
{
	int len;

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	return len;
}

void fileCopy(FILE *fromStream, FILE *toStream, int fileLen)
{
	char buf[BUF_SIZE];
	int count = 0;

	while (!feof(fromStream))
	{
		system("cls");

		fread((void *)buf, 1, BUF_SIZE, fromStream);
		fwrite((void *)buf, 1, BUF_SIZE, toStream);

		printf("%.2f%%\n", BUF_SIZE * count / (double)fileLen * 100);
		++count;
	}

	system("cls");
	printf("100.00\n");

	return;
}