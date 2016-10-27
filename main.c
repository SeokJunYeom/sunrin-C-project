#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <Windows.h>
#include <math.h>

#define BUF_SIZE 1024

/**************************** 형변환 ****************************/
int hexToInt(char * hexadecimal);

/**************************** 메뉴얼 ****************************/
void manual();

/*********************** cursor 관련 함수 ***********************/
void gotoxy(int x, int y);
int curinfoX();
int curinfoY();

/************************ file 관련 함수 ************************/
void makeHexFile(FILE *fp, char *fileName, char *boxName, int boxSize);
void nameofBox(FILE *fp, char *name);
int sizeofBox(FILE *fp);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		manual();
		exit(0);
	} // 인자 부족할 경우 메뉴얼 띄우고 프로그램 종료

	FILE *fromStream = NULL;

	char dir[BUF_SIZE] = { 0, };
	char fromFileName[BUF_SIZE] = { 0, };

	char boxName[5] = { 0, };
	int boxSize;

	getcwd(dir, BUF_SIZE);
	sprintf(fromFileName, "%s\\%s", dir, argv[1]);

	fromStream = fopen(fromFileName, "rb");

	if (fromStream == NULL)
	{
		printf("원본 파일을 찾을 수 없습니다.");
		exit(0);
	}

	else
	{
		while (!feof(fromStream))
		{
			boxSize = sizeofBox(fromStream);
			fseek(fromStream, 4, SEEK_CUR);

			nameofBox(fromStream, boxName);
			fseek(fromStream, -4, SEEK_CUR);

			makeHexFile(fromStream, fromFileName, boxName, boxSize);
			fseek(fromStream, boxSize, SEEK_CUR);
		}
	}

	fclose(fromStream);

	return 0;
}

void manual()
{
	printf("Usage : filecv \"FILE\"\n");
	printf("FILE  : mp4 FILE\n");
	printf("Parsing mp4 FILE.");
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

	return curinfo.dwCursorPosition.Y;
}

void makeHexFile(FILE *fp, char *fileName, char *boxName, int boxSize)
{
	FILE *hexfp = NULL;
	char filename[BUF_SIZE] = { 0, };

	int count = 0;

	sprintf(filename, "%s_%s.txt", fileName, boxName);

	hexfp = fopen(filename, "w");

	printf("%s 파일 생성 중...\n", boxName);

	int x = curinfoX();
	int y = curinfoY();

	while (count++ != boxSize)
	{
		gotoxy(x, y);

		fprintf(hexfp, "%02X ", fgetc(fp));
		printf("%d / %d (Byte)", count * 3, boxSize * 3);
	}

	fseek(fp, -1 * boxSize, SEEK_CUR);

	gotoxy(x, y);
	printf("%d / %d (Byte) 완료\n", boxSize * 3, boxSize * 3);
}

int sizeofBox(FILE *fp)
{
	char buf[4] = { 0, };
	char size_s[BUF_SIZE] = { 0, };
	int size;
	int count = 0;

	while (count++ != 4)
	{
		sprintf(buf, "%02X", fgetc(fp));
		strcat(size_s, buf);
	}

	fseek(fp, -4, SEEK_CUR);

	size = hexToInt(size_s);

	return size;
}

void nameofBox(FILE *fp, char *name)
{
	fread(name, 4, 1, fp);
	fseek(fp, -4, SEEK_CUR);
}

int hexToInt(char *hexadecimal)
{
	int decimal = 0;
	int position = 0;

	for (int i = strlen(hexadecimal) - 1; i >= 0; i--)
	{
		char ch = hexadecimal[i]; 

		if (ch >= 48 && ch <= 57)
			decimal += (ch - 48) * (int)pow(16, position);

		else if (ch >= 65 && ch <= 70)
			decimal += (ch - (65 - 10)) * (int)pow(16, position);

		else if (ch >= 97 && ch <= 102)										
			decimal += (ch - (97 - 10)) * (int)pow(16, position);

		position++;
	}

	return decimal;
}