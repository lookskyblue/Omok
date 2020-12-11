#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

#define WIN 5
#define MAX_X_COORD 17
#define MAX_Y_COORD 17

#define WALL -1
#define EMPTY 0
#define BLACK 1
#define WHITE 2

#define SPACE	32
#define LEFT	75
#define RIGHT	77
#define UP		72
#define DOWN	80
#define ESC		27

int  IsWin(int x, int y, int stone);
int  ConsiderStone(int stone, int isFive);

bool IsEmpty(int x, int y);
bool IsForbidden(int x, int y);
bool IsThreeThree(int x, int y);
bool IsFourFour(int x, int y);
bool IsSix(int x, int y);

void gotoxy(int x, int y);	// (0, 0) ~ (79, 24)
void IntroGame();
void SetWall();
void DrawField();
void GetKey();
void Warning(char* warnStr);
void SearchThreeThree(int x, int y, int xSign, int ySign, int* isTwiceThreeThree, int* isOverlap, int k);
void SearchFourFour(int x, int y, int xSign, int ySign, int* isTwiceFourFour);

int field[MAX_X_COORD][MAX_Y_COORD] = { 0 };
int(*pf)[MAX_Y_COORD] = field;

int main()
{
	char key = 0;
	while (tolower(key) != ESC)
	{
		system("cls");
		memset(&field, 0, sizeof(field));

		IntroGame();
		SetWall();
		DrawField();
		GetKey();

		puts("Continue:	Press C");
		puts("Stop:		Press ESC");

		key = _getch();
		while ((tolower(key) != ESC) && (tolower(key) != 'c'))
		{
			key = _getch();
		}
	}
	return 0;
}

void IntroGame()
{
	for (int i = 0; i < 5; i++, Sleep(250))
		printf("♪ ");
	puts("");
	puts("Omok version 2.7.3");
	puts("개발일자: 2020-09-01");
	printf("\n규칙\n");
	puts("흑돌이 선 입니다.");
	puts("렌주룰에 따라, 흑돌은 33, 44, 6목이 금수입니다.");
	puts("다만, 흑은 오목을 만들 때는 모든 금수를 무시합니다.");
	puts("백돌은 아무런 제약이 없습니다.");
	printf("\n조작법\n");
	puts("방향키:		이동");
	puts("스페이스바 :	착점");
	printf("\n인지하셨다면 키보드를 누르세요\n");
	_getch();
	system("cls");
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void DrawField()
{
	for (int i = 0; i < MAX_X_COORD; i++)
	{
		int k = 0;
		for (int j = 0; j < MAX_Y_COORD; j++)
		{
			gotoxy(k, i);
			k += 2;

			if (pf[j][i] == EMPTY)
				printf("┼");

			else if (pf[j][i] == BLACK)
				printf("○");

			else if (pf[j][i] == WHITE)
				printf("●");

			else if (pf[j][i] == WALL)
				printf(" ");
		}
	}
	gotoxy(2, 1);
}

bool IsEmpty(int x, int y)
{
	if (pf[x][y] == EMPTY)
		return true;

	else
	{
		Warning("Not Empty");
		return false;
	}
}

void GetKey()
{
	char key = 0;
	int stone = BLACK;
	int screenX = 2, screenY = 1;
	int myFieldX = 1, myFieldY = 1;

	while (1)
	{
		if (_kbhit())
		{
			key = _getch();
			switch (key)
			{
			case UP:
				if (myFieldY == 1)
					break;
				gotoxy(screenX, screenY--);		gotoxy(screenX, screenY);
				myFieldY--;
				break;

			case LEFT:
				if (myFieldX == 1)
					break;
				gotoxy(screenX -= 2, screenY);	gotoxy(screenX, screenY);
				myFieldX--;
				break;

			case DOWN:
				if (myFieldY == MAX_Y_COORD - 2)
					break;
				gotoxy(screenX, screenY++);		gotoxy(screenX, screenY);
				myFieldY++;
				break;

			case RIGHT:
				if (myFieldX == MAX_X_COORD - 2)
					break;
				gotoxy(screenX += 2, screenY);	gotoxy(screenX, screenY);
				myFieldX++;
				break;

			case SPACE:
				if (IsEmpty(myFieldX, myFieldY))
				{
					if (IsWin(myFieldX, myFieldY, stone))
					{
						pf[myFieldX][myFieldY] = stone;
						DrawField();
						gotoxy(0, 16);
						printf("%s WIN \n", stone == BLACK ? "BLACK ○" : "WHITE ●");
						return;
					}

					else if (stone == BLACK)
					{
						if (IsForbidden(myFieldX, myFieldY))
						{
							gotoxy(screenX, screenY);
							break;
						}
						pf[myFieldX][myFieldY] = BLACK;
					}

					else
						pf[myFieldX][myFieldY] = WHITE;

					DrawField();
					gotoxy(screenX, screenY);

					if (stone == BLACK)
						stone = WHITE;
					else
						stone = BLACK;
				}
				gotoxy(screenX, screenY);
			}
		}
	}
}
int ConsiderStone(int stone, int isFive)
{
	if (stone == WHITE)
	{
		if (isFive >= WIN)
			return 1;
	}
	else
		if (isFive == WIN)
			return 1;
	return 0;
}

int IsWin(int x, int y, int stone)
{
	int i;
	int isFive;

	for (i = 1, isFive = 1; pf[x + i][y] == stone; i++, isFive++);		// →
	for (i = 1; pf[x - i][y] == stone; i++, isFive++);					// ←
	if (ConsiderStone(stone, isFive))
		return 1;

	for (i = 1, isFive = 1; pf[x][y - i] == stone; i++, isFive++);		//↑
	for (i = 1; pf[x][y + i] == stone; i++, isFive++);					//↓
	if (ConsiderStone(stone, isFive))
		return 1;

	for (i = 1, isFive = 1;pf[x - i][y - i] == stone; i++, isFive++);	//↖
	for (i = 1; pf[x + i][y + i] == stone; i++, isFive++);				//↘
	if (ConsiderStone(stone, isFive))
		return 1;

	for (i = 1, isFive = 1; pf[x - i][y + i] == stone; i++, isFive++);	//↙
	for (i = 1; pf[x + i][y - i] == stone; i++, isFive++);				//↗
	if (ConsiderStone(stone, isFive))
		return 1;

	return 0;
}

bool IsForbidden(int x, int y)
{
	if (IsThreeThree(x, y) || IsFourFour(x, y) || IsSix(x, y))
		return true;
	else
		return false;
}

bool IsThreeThree(int x, int y)
{
	int isTwiceThreeThree = 0;
	int isOverlap[8] = { 0 };
	SearchThreeThree(x, y, 0, -1, &isTwiceThreeThree, isOverlap, 0);	// ↑
	SearchThreeThree(x, y, 0, 1, &isTwiceThreeThree, isOverlap, 1);	// ↓	
	SearchThreeThree(x, y, 1, 0, &isTwiceThreeThree, isOverlap, 2);	// →
	SearchThreeThree(x, y, -1, 0, &isTwiceThreeThree, isOverlap, 3);	// ←
	SearchThreeThree(x, y, -1, -1, &isTwiceThreeThree, isOverlap, 4);	// ↖
	SearchThreeThree(x, y, 1, 1, &isTwiceThreeThree, isOverlap, 5);	// ↘
	SearchThreeThree(x, y, 1, -1, &isTwiceThreeThree, isOverlap, 6);	// ↗
	SearchThreeThree(x, y, -1, 1, &isTwiceThreeThree, isOverlap, 7);	// ↙

	if (isTwiceThreeThree >= 2)
	{
		Warning("3-3");
		return true;
	}
	else
		return false;
}

void SearchThreeThree(int x, int y, int xSign, int ySign, int* isTwiceThreeThree, int* isOverlap, int k)
{
	if (isOverlap[k] == 1)
		return;

	int i;
	int countBlack = 0;

	if (pf[x + (xSign * (-1))][y + (ySign * (-1))] == BLACK)
	{
		if (pf[x + (xSign * (-2))][y + (ySign * (-2))] == EMPTY)
		{
			if (pf[x + (xSign * 1)][y + (ySign * 1)] == BLACK)
			{
				if (pf[x + (xSign * 2)][y + (ySign * 2)] == EMPTY)
				{
					countBlack = 3;
					isOverlap[k + 1] = 1;
				}
			}
			else if (pf[x + (xSign * 1)][y + (ySign * 1)] == EMPTY)
			{
				if (pf[x + (xSign * 2)][y + (ySign * 2)] == BLACK)
					if (pf[x + (xSign * 3)][y + (ySign * 3)] == EMPTY)
						countBlack = 3;
			}
		}
	}

	else if (pf[x + (xSign * (-1))][y + (ySign * (-1))] == EMPTY)
	{
		if (pf[x + (xSign * 1)][y + (ySign * 1)] == BLACK)
		{
			for (i = 2, countBlack = 2; i < 4; i++)
			{
				if (pf[x + (xSign * i)][y + (ySign * i)] == BLACK)
				{
					if (pf[x + (xSign * (i + 1))][y + (ySign * (i + 1))] != EMPTY)
						break;

					else
						countBlack++;
				}
			}
		}
		else if (pf[x + (xSign * 1)][y + (ySign * 1)] == EMPTY)
		{
			for (i = 2, countBlack = 1; i < 4; i++)
			{
				if (pf[x + (xSign * i)][y + (ySign * i)] != BLACK)
					break;
				else
					countBlack++;
				if (i == 3)
					if (pf[x + (xSign * (i + 1))][y + (ySign * (i + 1))] != EMPTY)
						return;
			}
		}

	}

	isOverlap[k] = 1;
	if (countBlack == 3)
		(*isTwiceThreeThree)++;
}

bool IsFourFour(int x, int y)
{
	int isTwiceFourFour = 0;
	SearchFourFour(x, y, 0, -1, &isTwiceFourFour);	// ↑
	SearchFourFour(x, y, 1, -1, &isTwiceFourFour);	// ↗
	SearchFourFour(x, y, 1, 0, &isTwiceFourFour);	// →
	SearchFourFour(x, y, 1, 1, &isTwiceFourFour);	// ↘
	SearchFourFour(x, y, 0, 1, &isTwiceFourFour);	// ↓
	SearchFourFour(x, y, -1, 1, &isTwiceFourFour);	// ↙
	SearchFourFour(x, y, -1, 0, &isTwiceFourFour);	// ←
	SearchFourFour(x, y, -1, -1, &isTwiceFourFour);	// ↖

	if (isTwiceFourFour >= 2)
	{
		Warning("4-4");
		return true;
	}
	else
		return false;
}

void SearchFourFour(int x, int y, int xSign, int ySign, int* isTwiceFourFour)
{
	int i;
	int countBlack = 0;

	if (pf[x + (xSign * (-1))][y + (ySign * (-1))] == BLACK)
	{
		if (pf[x + (xSign * (-2))][y + (ySign * (-2))] == WHITE || pf[x + (xSign * (-2))][y + (ySign * (-2))] == WALL)
			if (pf[x + (xSign * (1))][y + (ySign * (1))] == BLACK)
				if (pf[x + (xSign * (2))][y + (ySign * (2))] == BLACK)
					if (pf[x + (xSign * (3))][y + (ySign * (3))] == WHITE || pf[x + (xSign * (3))][y + (ySign * (3))] == WALL)
						return;

		for (i = 1, countBlack = 2; i < 4; i++)
		{
			if (pf[x + (xSign * i)][y + (ySign * i)] == BLACK)
				countBlack++;
			else if (pf[x + (xSign * i)][y + (ySign * i)] == WHITE || pf[x + (xSign * i)][y + (ySign * i)] == WALL)
				break;
		}
	}

	else
	{
		if (pf[x + (xSign * (-1))][y + (ySign * (-1))] == WHITE || pf[x + (xSign * (-1))][y + (ySign * (-1))] == WALL)
			if (pf[x + (xSign * (1))][y + (ySign * (1))] == BLACK)
				if (pf[x + (xSign * (2))][y + (ySign * (2))] == BLACK)
					if (pf[x + (xSign * (3))][y + (ySign * (3))] == BLACK)
						if (pf[x + (xSign * (4))][y + (ySign * (4))] == WHITE || pf[x + (xSign * (4))][y + (ySign * (4))] == WALL)
							return;

		for (i = 1, countBlack = 1; i < 5; i++)
		{
			if (pf[x + (xSign * i)][y + (ySign * i)] == BLACK)
				countBlack++;
			else if (pf[x + (xSign * i)][y + (ySign * i)] == WHITE || pf[x + (xSign * i)][y + (ySign * i)] == WALL)
				break;
		}
	}
	if (countBlack == 4)
		(*isTwiceFourFour)++;
}

bool IsSix(int x, int y)
{
	int i;
	int stone = BLACK;
	int isSix = 0;
	for (i = 1, isSix = 1; pf[x + i][y] == stone; i++, isSix++);		// →
	for (i = 1; pf[x - i][y] == stone; i++, isSix++);					// ←
	if (isSix >= 6)
		goto WARN;

	for (i = 1, isSix = 1; pf[x][y - i] == stone; i++, isSix++);		//↑
	for (i = 1; pf[x][y + i] == stone; i++, isSix++);					//↓
	if (isSix >= 6)
		goto WARN;

	for (i = 1, isSix = 1;pf[x - i][y - i] == stone; i++, isSix++);		//↖
	for (i = 1; pf[x + i][y + i] == stone; i++, isSix++);				//↘
	if (isSix >= 6)
		goto WARN;

	for (i = 1, isSix = 1; pf[x - i][y + i] == stone; i++, isSix++);	//↙
	for (i = 1; pf[x + i][y - i] == stone; i++, isSix++);				//↗
	if (isSix >= 6)
		goto WARN;

	return false;

WARN:
	Warning("6 stone");
	return true;
}

void Warning(char* warnStr)
{
	Beep(600, 200);
	gotoxy(33, 1);
	puts(warnStr);
	Sleep(500);
	system("cls");
	DrawField();
}

void SetWall()
{
	int i;

	for (i = 0; i < MAX_X_COORD; i++)
		pf[i][0] = WALL;

	for (i = 0; i < MAX_Y_COORD - 1; i++)
	{
		pf[0][i] = WALL;
		pf[MAX_X_COORD - 1][i] = WALL;
	}

	for (i = 0; i < MAX_X_COORD; i++)
		pf[i][MAX_Y_COORD - 1] = WALL;
}