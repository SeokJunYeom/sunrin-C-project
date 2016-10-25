#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <Windows.h>

#define BUF_SIZE 1024

/**************************** 메뉴얼 ****************************/
void manual();

/*********************** cursor 관련 함수 ***********************/
void gotoxy(int x, int y);
int curinfoX();
int curinfoY();

/************************ file 관련 함수 ************************/
void fileCopy(FILE *fromStream, FILE *toStream, __int64 fileLen);
__int64 getFileLen(FILE *fp);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		manual();
		exit(0);
	} // 인자 부족할 경우 메뉴얼 띄우고 프로그램 종료

	else if (!strcmp(argv[1], argv[2]))
	{
		manual();
		exit(0);
	} // 원본 파일명과 대상 파일명이 같을 경우 메뉴얼 띄우고 프로그램 종료

	FILE *fromStream = NULL;
	FILE *toStream = NULL;

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

		fileCopy(fromStream, toStream, getFileLen(fromStream));

		fclose(fromStream);
		fclose(toStream);
	}

	return 0;
}

void manual()
{
	printf("Usage  : filecv \"SOURCE\" \"DEST\"\n");
	printf("SOURCE : mp4 FILE\n");
	printf("DEST   : mp3 FILE\n");
	printf("Convert mp4 file to mp3 file.");
}

void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int curinfoX()
{
	CONSOLE_SCREEN_BUFFER_INFO curinfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curinfo);

	return curinfo.dwCursorPosition.X;
}

int curinfoY()
{
	CONSOLE_SCREEN_BUFFER_INFO curinfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curinfo);

	return curinfo.dwCursorPosition.X;
}

void fileCopy(FILE *fromStream, FILE *toStream, __int64 fileLen)
{
	char buf[BUF_SIZE];
	int count = 0;

	int x = curinfoX();
	int y = curinfoY();

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

__int64 getFileLen(FILE *fp)
{
	fpos_t pos;

	fseek(fp, 0, SEEK_END);
	fgetpos(fp, &pos);
	fseek(fp, 0, SEEK_SET);

	return pos;
}