#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include "direction.h"

#define FIVE_STONE 5
#define MAX_X_COORD 17
#define MAX_Y_COORD 17
#define WALL -1
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define SPACE 32
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27

void SearchThreeThree(int x, int y, int directionX, int directionY, int* const isTwiceThreeThree, int* const isOverlap, int k);
void SearchFourFour(int x, int y, int directionX, int directionY, int* const isTwiceFourFour);
void ShowWarning(const char* const warnStr);
void ShowGameOver(int myStoneColor);
void ShowGameIntro();
void ShowBoard();
void SetBoardWall();
void GetKeyInput();
void GoToXY(int x, int y);
bool IsMoreThanFiveStone(int myStoneColor, int numOfMyStones);
bool IsWin(int x, int y, int myStoneColor);
bool IsEmpty(int x, int y);
bool IsForbidden(int x, int y);
bool IsThreeThree(int x, int y);
bool IsFourFour(int x, int y);
bool IsSix(int x, int y);
int  ChangeTurn(int myStoneColor);

int board[MAX_X_COORD][MAX_Y_COORD] = { 0 };
int(*board_ptr)[MAX_X_COORD] = board;
int NumOfDirection = sizeof(searching_Win_Dir) / sizeof(searching_Win_Dir[0]);

int main()
{
	char keyInput = 0;

	while (keyInput != ESC)
	{
		system("cls");
		memset(&board, 0, sizeof(board));
		ShowGameIntro();
		SetBoardWall();
		ShowBoard();
		GetKeyInput();

		do
		{
			keyInput = _getch();
		} while ((keyInput != ESC) && (tolower(keyInput) != 'c'));
	}

	return 0;
}

void ShowGameIntro()
{
	for (int i = 0; i < 5; i++, Sleep(200))
		printf("¢Ü ");

	printf("\n\n__Rule__\n");
	puts("Black stone first.");
	puts("As renju rule, black stone is forbidden to have 33, 44, 6 stones.");
	puts("But black stone can ignore those rule when making 5 stones.");
	puts("White stones don't have any restrictions.");
	printf("\n\n__Manual__\n");
	puts("Arrow keys: move");
	puts("space bar : put stone");
	printf("\nIf you recognize it, press the keyboard.\n");
	_getch();
	system("cls");
}

void GoToXY(int x, int y)
{
	COORD pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void ShowBoard()
{
	int x;

	for (int i = 0; i < MAX_X_COORD; i++)
	{
		x = 0;

		for (int j = 0; j < MAX_Y_COORD; j++)
		{
			GoToXY(x, i);
			x += 1;

			if (board_ptr[j][i] == EMPTY)
			{
				printf("¦«");
			}

			else if (board_ptr[j][i] == BLACK)
			{
				printf("¡Û");
			}

			else if (board_ptr[j][i] == WHITE)
			{
				printf("¡Ü");
			}

			else if (board_ptr[j][i] == WALL)
			{
				printf(" ");
			}
		}
	}

	GoToXY(1, 1);
}

bool IsEmpty(int x, int y)
{
	if (board_ptr[x][y] == EMPTY)
	{
		return true;
	}

	else
	{
		ShowWarning("Not Empty");

		return false;
	}
}

void GetKeyInput()
{
	char keyInput = 0;
	int myStoneColor = BLACK;
	int screenCursorX = 1, screenCursorY = 1;

	while (1)
	{
		if (_kbhit())
		{
			keyInput = _getch();

			switch (keyInput)
			{
			case UP:
			{
				if (screenCursorY == 1)
				{
					break;
				}

				GoToXY(screenCursorX, --screenCursorY);
				break;
			}

			case LEFT:
			{
				if (screenCursorX == 1)
				{
					break;
				}

				GoToXY(--screenCursorX, screenCursorY);
				break;
			}

			case DOWN:
			{
				if (screenCursorY == MAX_Y_COORD - 2)
				{
					break;
				}

				GoToXY(screenCursorX, ++screenCursorY);
				break;
			}

			case RIGHT:
			{
				if (screenCursorX == MAX_X_COORD - 2)
				{
					break;
				}

				GoToXY(++screenCursorX, screenCursorY);
				break;
			}

			case SPACE:
			{
				if (IsEmpty(screenCursorX, screenCursorY) == false)
				{
					GoToXY(screenCursorX, screenCursorY);
				}

				else
				{
					if (IsWin(screenCursorX, screenCursorY, myStoneColor) == true)
					{
						board_ptr[screenCursorX][screenCursorY] = myStoneColor;

						ShowBoard();
						ShowGameOver(myStoneColor);

						return;
					}

					else if (myStoneColor == BLACK)
					{
						if (IsForbidden(screenCursorX, screenCursorY))
						{
							GoToXY(screenCursorX, screenCursorY);
							break;
						}

						board_ptr[screenCursorX][screenCursorY] = BLACK;
					}

					else
					{
						board_ptr[screenCursorX][screenCursorY] = WHITE;
					}

					ShowBoard();
					GoToXY(screenCursorX, screenCursorY);
					myStoneColor = ChangeTurn(myStoneColor);
				}
			}

			default:
				break;
			}
		}
	}
}

bool IsMoreThanFiveStone(int myStoneColor, int numOfMyStones)
{
	if (myStoneColor == WHITE)
	{
		if (numOfMyStones >= FIVE_STONE)
		{
			return true;
		}
	}

	else
	{
		if (numOfMyStones == FIVE_STONE)
		{
			return true;
		}
	}

	return false;
}

bool IsWin(int x, int y, int myStoneColor)
{
	int i;
	int numOfMyStones;

	for (int j = 0; j < NumOfDirection; j++)
	{
		for (i = 1, numOfMyStones = 1;
			board_ptr[x + (i * (searching_Win_Dir[j][0][0])) * searching_Win_Dir[j][0][1]][y + (i * (searching_Win_Dir[j][0][2])) * searching_Win_Dir[j][0][3]] == myStoneColor;
			i++, numOfMyStones++);

		for (i = 1;
			board_ptr[x + (i * (searching_Win_Dir[j][1][0])) * searching_Win_Dir[j][1][1]][y + (i * (searching_Win_Dir[j][1][2])) * searching_Win_Dir[j][1][3]] == myStoneColor;
			i++, numOfMyStones++);

		if (IsMoreThanFiveStone(myStoneColor, numOfMyStones) == true)
		{
			return true;
		}
	}

	return false;
}

bool IsForbidden(int x, int y)
{
	if (IsThreeThree(x, y) || IsFourFour(x, y) || IsSix(x, y))
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool IsThreeThree(int x, int y)
{
	int countThreeThree = 0;
	int isOverlap[8] = { 0 };

	for (int i = 0; i < 8; i++)
	{
		SearchThreeThree(x, y, searching_ThreeThree_Dir[i][0], searching_ThreeThree_Dir[i][1], &countThreeThree, isOverlap, i);

		if (countThreeThree >= 2)
		{
			ShowWarning("3-3");

			return true;
		}
	}

	return false;
}

void SearchThreeThree(int x, int y, int directionX, int directionY, int* const countThreeThree, int* const isOverlap, int k)
{
	if (isOverlap[k] == 1)
	{
		return;
	}

	int i;
	int countBlack = 0;

	if (board_ptr[x + (directionX * (-1))][y + (directionY * (-1))] == BLACK)
	{
		if (board_ptr[x + (directionX * (-2))][y + (directionY * (-2))] == EMPTY)
		{
			if (board_ptr[x + (directionX * 1)][y + (directionY * 1)] == BLACK)
			{
				if (board_ptr[x + (directionX * 2)][y + (directionY * 2)] == EMPTY)
				{
					countBlack = 3;
					isOverlap[k + 1] = 1;
				}
			}

			else if (board_ptr[x + (directionX * 1)][y + (directionY * 1)] == EMPTY)
			{
				if (board_ptr[x + (directionX * 2)][y + (directionY * 2)] == BLACK)
				{
					if (board_ptr[x + (directionX * 3)][y + (directionY * 3)] == EMPTY)
					{
						countBlack = 3;
					}
				}
			}
		}
	}

	else if (board_ptr[x + (directionX * (-1))][y + (directionY * (-1))] == EMPTY)
	{
		if (board_ptr[x + (directionX * 1)][y + (directionY * 1)] == BLACK)
		{
			for (i = 2, countBlack = 2; i < 4; i++)
			{
				if (board_ptr[x + (directionX * i)][y + (directionY * i)] == BLACK)
				{
					if (board_ptr[x + (directionX * (i + 1))][y + (directionY * (i + 1))] != EMPTY)
					{
						break;
					}

					else
					{
						countBlack++;
					}
				}
			}
		}

		else if (board_ptr[x + (directionX * 1)][y + (directionY * 1)] == EMPTY)
		{
			for (i = 2, countBlack = 1; i < 4; i++)
			{
				if (board_ptr[x + (directionX * i)][y + (directionY * i)] != BLACK)
				{
					break;
				}

				else
				{
					countBlack++;
				}

				if (i == 3)
				{
					if (board_ptr[x + (directionX * (i + 1))][y + (directionY * (i + 1))] != EMPTY)
					{
						return;
					}
				}
			}
		}
	}

	isOverlap[k] = 1;

	if (countBlack == 3)
	{
		(*countThreeThree)++;
	}
}

bool IsFourFour(int x, int y)
{
	int countFourFour = 0;

	for (int i = 0; i < 8; i++)
	{
		SearchFourFour(x, y, searching_FourFour_Dir[i][0], searching_FourFour_Dir[i][1], &countFourFour);

		if (countFourFour >= 2)
		{
			ShowWarning("4-4");

			return true;
		}
	}

	return false;
}

void SearchFourFour(int x, int y, int directionX, int directionY, int* const countFourFour)
{
	int i;
	int countBlack = 0;

	if (board_ptr[x + (directionX * (-1))][y + (directionY * (-1))] == BLACK)
	{
		if (board_ptr[x + (directionX * (-2))][y + (directionY * (-2))] == WHITE || board_ptr[x + (directionX * (-2))][y + (directionY * (-2))] == WALL)
		{
			if (board_ptr[x + (directionX * (1))][y + (directionY * (1))] == BLACK)
			{
				if (board_ptr[x + (directionX * (2))][y + (directionY * (2))] == BLACK)
				{
					if (board_ptr[x + (directionX * (3))][y + (directionY * (3))] == WHITE || board_ptr[x + (directionX * (3))][y + (directionY * (3))] == WALL)
					{
						return;
					}
				}
			}
		}

		for (i = 1, countBlack = 2; i < 4; i++)
		{
			if (board_ptr[x + (directionX * i)][y + (directionY * i)] == BLACK)
			{
				countBlack++;
			}

			else if (board_ptr[x + (directionX * i)][y + (directionY * i)] == WHITE || board_ptr[x + (directionX * i)][y + (directionY * i)] == WALL)
			{
				break;
			}
		}
	}

	else
	{
		if (board_ptr[x + (directionX * (-1))][y + (directionY * (-1))] == WHITE || board_ptr[x + (directionX * (-1))][y + (directionY * (-1))] == WALL)
		{
			if (board_ptr[x + (directionX * (1))][y + (directionY * (1))] == BLACK)
			{
				if (board_ptr[x + (directionX * (2))][y + (directionY * (2))] == BLACK)
				{
					if (board_ptr[x + (directionX * (3))][y + (directionY * (3))] == BLACK)
					{
						if (board_ptr[x + (directionX * (4))][y + (directionY * (4))] == WHITE || board_ptr[x + (directionX * (4))][y + (directionY * (4))] == WALL)
						{
							return;
						}
					}
				}
			}
		}

		for (i = 1, countBlack = 1; i < 5; i++)
		{
			if (board_ptr[x + (directionX * i)][y + (directionY * i)] == BLACK)
			{
				countBlack++;
			}

			else if (board_ptr[x + (directionX * i)][y + (directionY * i)] == WHITE || board_ptr[x + (directionX * i)][y + (directionY * i)] == WALL)
			{
				break;
			}
		}
	}

	if (countBlack == 4)
	{
		(*countFourFour)++;
	}
}

bool IsSix(int x, int y)
{
	int i;
	int myStoneColor = BLACK;
	int numOfMyStones = 0;

	for (int j = 0; j < NumOfDirection; j++)
	{
		for (i = 1, numOfMyStones = 1;
			board_ptr[x + (i * (searching_Win_Dir[j][0][0])) * searching_Win_Dir[j][0][1]][y + (i * (searching_Win_Dir[j][0][2])) * searching_Win_Dir[j][0][3]] == myStoneColor;
			i++, numOfMyStones++);

		for (i = 1;
			board_ptr[x + (i * (searching_Win_Dir[j][1][0])) * searching_Win_Dir[j][1][1]][y + (i * (searching_Win_Dir[j][1][2])) * searching_Win_Dir[j][1][3]] == myStoneColor;
			i++, numOfMyStones++);

		if (numOfMyStones >= 6)
		{
			ShowWarning("6 Stone");

			return true;
		}
	}

	return false;
}

void ShowWarning(const char* const warnStr)
{
	Beep(600, 200);
	GoToXY(17, 1);
	puts(warnStr);
	Sleep(300);
	system("cls");
	ShowBoard();
}

void ShowGameOver(int myStoneColor)
{
	GoToXY(0, 16);
	printf("%s WIN \n", myStoneColor == BLACK ? "BLACK ¡Û" : "WHITE ¡Ü");
	puts("Continue: Press C");
	puts("Stop:     Press ESC");
}

int ChangeTurn(int myStoneColor)
{
	if (myStoneColor == BLACK)
	{
		return WHITE;
	}

	else
	{
		return BLACK;
	}
}

void SetBoardWall()
{
	int i;

	for (i = 0; i < MAX_X_COORD; i++)
	{
		board_ptr[i][0] = WALL;
		board_ptr[i][MAX_Y_COORD - 1] = WALL;
		board_ptr[0][i] = WALL;
		board_ptr[MAX_X_COORD - 1][i] = WALL;
	}
}