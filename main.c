#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <Windows.h>

#define BUF_SIZE 1024

void manual();
void gotoxy(int x, int y);
CONSOLE_SCREEN_BUFFER_INFO curInfo();
void fileCopy(FILE *fromStream, FILE *toStream, long long fileLen);
long long getFileLen(FILE *fp);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		manual();
		exit(0);
	}

	else if (!strcmp(argv[1], argv[2]))
	{
		manual();
		exit(0);
	}

	FILE *fromStream = NULL;
	FILE *toStream = NULL;

	long long fileLen;

	char dir[BUF_SIZE] = { 0, };
	char fromFileName[BUF_SIZE] = { 0, };
	char toFileName[BUF_SIZE] = { 0, };

	getcwd(dir, BUF_SIZE);
	sprintf(fromFileName, "%s\\%s", dir, argv[1]);
	sprintf(toFileName, "%s\\%s", dir, argv[2]);

	fromStream = fopen(fromFileName, "rb");

	if (fromStream == NULL)
	{
		printf("원본 파일을 찾을 수 없습니다.");
		exit(0);
	}

	else
	{
		toStream = fopen(toFileName, "wb");

		fileLen = getFileLen(fromStream);

		fileCopy(fromStream, toStream, fileLen);

		fclose(fromStream);
		fclose(toStream);
	}

	return 0;
}

void manual()
{
	printf("Usage : ~~~~~~~~~~~~");
}

void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

CONSOLE_SCREEN_BUFFER_INFO curInfo()
{
	CONSOLE_SCREEN_BUFFER_INFO curInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

	return curInfo;
}

void fileCopy(FILE *fromStream, FILE *toStream, long long fileLen)
{
	char buf[BUF_SIZE];
	int count = 0;
	
	int x = curInfo().dwCursorPosition.X;
	int y = curInfo().dwCursorPosition.Y;

	while (!feof(fromStream))
	{
		gotoxy(x, y);

		fread((void *)buf, 1, BUF_SIZE, fromStream);
		fwrite((void *)buf, 1, BUF_SIZE, toStream);

		printf("%d / %lld (Byte)", count * BUF_SIZE, fileLen);
		++count;
	}

	gotoxy(x, y);
	printf("%lld / %lld (Byte) 완료", fileLen, fileLen);
}

long long getFileLen(FILE *fp)
{
	fpos_t pos;

	fseek(fp, 0, SEEK_END);
	fgetpos(fp, &pos);
	fseek(fp, 0, SEEK_SET);

	return pos;
}