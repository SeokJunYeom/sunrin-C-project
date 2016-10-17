#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define BUF_SIZE 1024

void gotoxy(int x, int y);
long long getFileLen(FILE *fp);
void fileCopy(FILE *fromStream, FILE *toStream, long long fileLen);

int main()
{
	FILE *fromStream = NULL;
	FILE *toStream = NULL;

	long long fileLen;

	char fromFileName[] = "D:\\Users\\PC81\\Desktop\\test.mkv";
	char toFileName[] = "D:\\Users\\PC81\\Desktop\\test2.mkv";

	fromStream = fopen(fromFileName, "rb");
	toStream = fopen(toFileName, "wb");

	if (fromStream == NULL || toStream == NULL)
	{
		printf("file open error\n");
		exit(0);
	}

	fileLen = getFileLen(fromStream);

	fileCopy(fromStream, toStream, fileLen);

	fclose(fromStream);
	fclose(toStream);

	return 0;
}

void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

long long getFileLen(FILE *fp)
{
	fpos_t pos;

	fseek(fp, 0, SEEK_END);
	fgetpos(fp, &pos);
	fseek(fp, 0, SEEK_SET);

	return pos;
}

void fileCopy(FILE *fromStream, FILE *toStream, long long fileLen)
{
	char buf[BUF_SIZE];
	int count = 0;

	while (!feof(fromStream))
	{;
		gotoxy(0, 0);

		fread((void *)buf, 1, BUF_SIZE, fromStream);
		fwrite((void *)buf, 1, BUF_SIZE, toStream);

		printf("%d / %lld (Byte)", count * BUF_SIZE, fileLen);
		++count;
	}

	gotoxy(0, 0);
	printf("%lld / %lld (Byte) ¿Ï·á\n", fileLen, fileLen);

	return;
}