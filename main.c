#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define BUF_SIZE 1024

void gotoxy(int x, int y);
double fileLen(FILE *fp);
void fileCopy(FILE *fromStream, FILE *toStream, double fileLen);

int main()
{
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;

	char fromFileName[] = "D:\\Users\\PC81\\Desktop\\test.jpg";
	char toFileName[] = "D:\\Users\\PC81\\Desktop\\test2.jpg";
	double len;

	fp1 = fopen(fromFileName, "rb");
	fp2 = fopen(toFileName, "wb");

	len = fileLen(fp1);

	fileCopy(fp1, fp2, len);

	fclose(fp1);
	fclose(fp2);

	return 0;
}

void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

double fileLen(FILE *fp)
{
	double len;

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	return len;
}

void fileCopy(FILE *fromStream, FILE *toStream, double fileLen)
{
	char buf[BUF_SIZE];
	int count = 0;

	while (!feof(fromStream))
	{
		gotoxy(0, 0);

		fread((void *)buf, 1, BUF_SIZE, fromStream);
		fwrite((void *)buf, 1, BUF_SIZE, toStream);

		printf(" %.2f%%\n", BUF_SIZE * count / fileLen * 100);
		++count;
	}

	gotoxy(0, 0);
	printf("100.00%%\n");

	return;
}