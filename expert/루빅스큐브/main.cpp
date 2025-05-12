#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define FRONT 0
#define BACK 1
#define UP 2
#define DOWN 3
#define LEFT 4
#define RIGHT 5

int Cube[6][3][3];
int rotate_cnt, total_score = 0, total_time = 0;

extern void runtest(int arr[6][3][3]);

void rotate(int face, bool clockwise)
{
	rotate_cnt++;

	int preCube[6][3][3];
	register int s, i, j;
	for (s = 0; s < 6; s++)
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				preCube[s][i][j] = Cube[s][i][j];

	if (clockwise)
	{
		Cube[face][0][0] = preCube[face][2][0];
		Cube[face][0][1] = preCube[face][1][0];
		Cube[face][0][2] = preCube[face][0][0];
		Cube[face][1][2] = preCube[face][0][1];
		Cube[face][2][2] = preCube[face][0][2];
		Cube[face][2][1] = preCube[face][1][2];
		Cube[face][2][0] = preCube[face][2][2];
		Cube[face][1][0] = preCube[face][2][1];
	}
	else
	{
		Cube[face][0][0] = preCube[face][0][2];
		Cube[face][0][1] = preCube[face][1][2];
		Cube[face][0][2] = preCube[face][2][2];
		Cube[face][1][2] = preCube[face][2][1];
		Cube[face][2][2] = preCube[face][2][0];
		Cube[face][2][1] = preCube[face][1][0];
		Cube[face][2][0] = preCube[face][0][0];
		Cube[face][1][0] = preCube[face][0][1];
	}

	switch (face)
	{
	case FRONT:
		if (clockwise)
		{
			Cube[UP][2][0] = preCube[LEFT][2][2];
			Cube[UP][2][1] = preCube[LEFT][1][2];
			Cube[UP][2][2] = preCube[LEFT][0][2];
			Cube[RIGHT][0][0] = preCube[UP][2][0];
			Cube[RIGHT][1][0] = preCube[UP][2][1];
			Cube[RIGHT][2][0] = preCube[UP][2][2];
			Cube[DOWN][0][2] = preCube[RIGHT][0][0];
			Cube[DOWN][0][1] = preCube[RIGHT][1][0];
			Cube[DOWN][0][0] = preCube[RIGHT][2][0];
			Cube[LEFT][2][2] = preCube[DOWN][0][2];
			Cube[LEFT][1][2] = preCube[DOWN][0][1];
			Cube[LEFT][0][2] = preCube[DOWN][0][0];
		}
		else
		{
			Cube[UP][2][0] = preCube[RIGHT][0][0];
			Cube[UP][2][1] = preCube[RIGHT][1][0];
			Cube[UP][2][2] = preCube[RIGHT][2][0];
			Cube[RIGHT][0][0] = preCube[DOWN][0][2];
			Cube[RIGHT][1][0] = preCube[DOWN][0][1];
			Cube[RIGHT][2][0] = preCube[DOWN][0][0];
			Cube[DOWN][0][2] = preCube[LEFT][2][2];
			Cube[DOWN][0][1] = preCube[LEFT][1][2];
			Cube[DOWN][0][0] = preCube[LEFT][0][2];
			Cube[LEFT][2][2] = preCube[UP][2][0];
			Cube[LEFT][1][2] = preCube[UP][2][1];
			Cube[LEFT][0][2] = preCube[UP][2][2];
		}
		break;
	case BACK:
		if (clockwise)
		{
			Cube[UP][0][0] = preCube[RIGHT][0][2];
			Cube[UP][0][1] = preCube[RIGHT][1][2];
			Cube[UP][0][2] = preCube[RIGHT][2][2];
			Cube[RIGHT][0][2] = preCube[DOWN][2][2];
			Cube[RIGHT][1][2] = preCube[DOWN][2][1];
			Cube[RIGHT][2][2] = preCube[DOWN][2][0];
			Cube[DOWN][2][2] = preCube[LEFT][2][0];
			Cube[DOWN][2][1] = preCube[LEFT][1][0];
			Cube[DOWN][2][0] = preCube[LEFT][0][0];
			Cube[LEFT][2][0] = preCube[UP][0][0];
			Cube[LEFT][1][0] = preCube[UP][0][1];
			Cube[LEFT][0][0] = preCube[UP][0][2];
		}
		else
		{
			Cube[UP][0][0] = preCube[LEFT][2][0];
			Cube[UP][0][1] = preCube[LEFT][1][0];
			Cube[UP][0][2] = preCube[LEFT][0][0];
			Cube[RIGHT][0][2] = preCube[UP][0][0];
			Cube[RIGHT][1][2] = preCube[UP][0][1];
			Cube[RIGHT][2][2] = preCube[UP][0][2];
			Cube[DOWN][2][2] = preCube[RIGHT][0][2];
			Cube[DOWN][2][1] = preCube[RIGHT][1][2];
			Cube[DOWN][2][0] = preCube[RIGHT][2][2];
			Cube[LEFT][2][0] = preCube[DOWN][2][2];
			Cube[LEFT][1][0] = preCube[DOWN][2][1];
			Cube[LEFT][0][0] = preCube[DOWN][2][0];
		}
		break;
	case UP:
		if (clockwise)
		{
			Cube[FRONT][0][0] = preCube[RIGHT][0][0];
			Cube[FRONT][0][1] = preCube[RIGHT][0][1];
			Cube[FRONT][0][2] = preCube[RIGHT][0][2];
			Cube[LEFT][0][0] = preCube[FRONT][0][0];
			Cube[LEFT][0][1] = preCube[FRONT][0][1];
			Cube[LEFT][0][2] = preCube[FRONT][0][2];
			Cube[BACK][0][0] = preCube[LEFT][0][0];
			Cube[BACK][0][1] = preCube[LEFT][0][1];
			Cube[BACK][0][2] = preCube[LEFT][0][2];
			Cube[RIGHT][0][0] = preCube[BACK][0][0];
			Cube[RIGHT][0][1] = preCube[BACK][0][1];
			Cube[RIGHT][0][2] = preCube[BACK][0][2];
		}
		else
		{
			Cube[FRONT][0][0] = preCube[LEFT][0][0];
			Cube[FRONT][0][1] = preCube[LEFT][0][1];
			Cube[FRONT][0][2] = preCube[LEFT][0][2];
			Cube[LEFT][0][0] = preCube[BACK][0][0];
			Cube[LEFT][0][1] = preCube[BACK][0][1];
			Cube[LEFT][0][2] = preCube[BACK][0][2];
			Cube[BACK][0][0] = preCube[RIGHT][0][0];
			Cube[BACK][0][1] = preCube[RIGHT][0][1];
			Cube[BACK][0][2] = preCube[RIGHT][0][2];
			Cube[RIGHT][0][0] = preCube[FRONT][0][0];
			Cube[RIGHT][0][1] = preCube[FRONT][0][1];
			Cube[RIGHT][0][2] = preCube[FRONT][0][2];
		}
		break;
	case DOWN:
		if (clockwise)
		{
			Cube[FRONT][2][0] = preCube[LEFT][2][0];
			Cube[FRONT][2][1] = preCube[LEFT][2][1];
			Cube[FRONT][2][2] = preCube[LEFT][2][2];
			Cube[LEFT][2][0] = preCube[BACK][2][0];
			Cube[LEFT][2][1] = preCube[BACK][2][1];
			Cube[LEFT][2][2] = preCube[BACK][2][2];
			Cube[BACK][2][0] = preCube[RIGHT][2][0];
			Cube[BACK][2][1] = preCube[RIGHT][2][1];
			Cube[BACK][2][2] = preCube[RIGHT][2][2];
			Cube[RIGHT][2][0] = preCube[FRONT][2][0];
			Cube[RIGHT][2][1] = preCube[FRONT][2][1];
			Cube[RIGHT][2][2] = preCube[FRONT][2][2];
		}
		else
		{
			Cube[FRONT][2][0] = preCube[RIGHT][2][0];
			Cube[FRONT][2][1] = preCube[RIGHT][2][1];
			Cube[FRONT][2][2] = preCube[RIGHT][2][2];
			Cube[LEFT][2][0] = preCube[FRONT][2][0];
			Cube[LEFT][2][1] = preCube[FRONT][2][1];
			Cube[LEFT][2][2] = preCube[FRONT][2][2];
			Cube[BACK][2][0] = preCube[LEFT][2][0];
			Cube[BACK][2][1] = preCube[LEFT][2][1];
			Cube[BACK][2][2] = preCube[LEFT][2][2];
			Cube[RIGHT][2][0] = preCube[BACK][2][0];
			Cube[RIGHT][2][1] = preCube[BACK][2][1];
			Cube[RIGHT][2][2] = preCube[BACK][2][2];
		}
		break;
	case LEFT:
		if (clockwise)
		{
			Cube[FRONT][0][0] = preCube[UP][0][0];
			Cube[FRONT][1][0] = preCube[UP][1][0];
			Cube[FRONT][2][0] = preCube[UP][2][0];
			Cube[UP][0][0] = preCube[BACK][2][2];
			Cube[UP][1][0] = preCube[BACK][1][2];
			Cube[UP][2][0] = preCube[BACK][0][2];
			Cube[BACK][2][2] = preCube[DOWN][0][0];
			Cube[BACK][1][2] = preCube[DOWN][1][0];
			Cube[BACK][0][2] = preCube[DOWN][2][0];
			Cube[DOWN][0][0] = preCube[FRONT][0][0];
			Cube[DOWN][1][0] = preCube[FRONT][1][0];
			Cube[DOWN][2][0] = preCube[FRONT][2][0];
		}
		else
		{
			Cube[FRONT][0][0] = preCube[DOWN][0][0];
			Cube[FRONT][1][0] = preCube[DOWN][1][0];
			Cube[FRONT][2][0] = preCube[DOWN][2][0];
			Cube[UP][0][0] = preCube[FRONT][0][0];
			Cube[UP][1][0] = preCube[FRONT][1][0];
			Cube[UP][2][0] = preCube[FRONT][2][0];
			Cube[BACK][2][2] = preCube[UP][0][0];
			Cube[BACK][1][2] = preCube[UP][1][0];
			Cube[BACK][0][2] = preCube[UP][2][0];
			Cube[DOWN][0][0] = preCube[BACK][2][2];
			Cube[DOWN][1][0] = preCube[BACK][1][2];
			Cube[DOWN][2][0] = preCube[BACK][0][2];
		}
		break;
	case RIGHT:
		if (clockwise)
		{
			Cube[FRONT][0][2] = preCube[DOWN][0][2];
			Cube[FRONT][1][2] = preCube[DOWN][1][2];
			Cube[FRONT][2][2] = preCube[DOWN][2][2];
			Cube[UP][0][2] = preCube[FRONT][0][2];
			Cube[UP][1][2] = preCube[FRONT][1][2];
			Cube[UP][2][2] = preCube[FRONT][2][2];
			Cube[BACK][2][0] = preCube[UP][0][2];
			Cube[BACK][1][0] = preCube[UP][1][2];
			Cube[BACK][0][0] = preCube[UP][2][2];
			Cube[DOWN][0][2] = preCube[BACK][2][0];
			Cube[DOWN][1][2] = preCube[BACK][1][0];
			Cube[DOWN][2][2] = preCube[BACK][0][0];
		}
		else
		{
			Cube[FRONT][0][2] = preCube[UP][0][2];
			Cube[FRONT][1][2] = preCube[UP][1][2];
			Cube[FRONT][2][2] = preCube[UP][2][2];
			Cube[UP][0][2] = preCube[BACK][2][0];
			Cube[UP][1][2] = preCube[BACK][1][0];
			Cube[UP][2][2] = preCube[BACK][0][0];
			Cube[BACK][2][0] = preCube[DOWN][0][2];
			Cube[BACK][1][0] = preCube[DOWN][1][2];
			Cube[BACK][0][0] = preCube[DOWN][2][2];
			Cube[DOWN][0][2] = preCube[FRONT][0][2];
			Cube[DOWN][1][2] = preCube[FRONT][1][2];
			Cube[DOWN][2][2] = preCube[FRONT][2][2];
		}
		break;
	}
}

int main()
{
	srand(3);
	for (int tc = 1; tc <= 1; tc++)		// 원래는 20개
	{
		register int s, i, j;
		for (s = 0; s < 6; s++)
			for (i = 0; i < 3; i++)
				for (j = 0; j < 3; j++)
					Cube[s][i][j] = s;

		for (i = 0; i < 10; i++) rotate(rand() % 6, rand() % 2);

		int arr[6][3][3];
		for (s = 0; s < 6; s++)
			for (i = 0; i < 3; i++)
				for (j = 0; j < 3; j++)
					arr[s][i][j] = Cube[s][i][j];

		rotate_cnt = 0;
		clock_t t1 = clock();
		runtest(arr);
		int sub_time = clock() - t1;
		int sub_score = rotate_cnt;

		for (s = 0; s < 6; s++)
			for (i = 0; i < 3; i++)
				for (j = 0; j < 3; j++)
					if (Cube[s][i][j] != s)
						sub_score += 1000;

		// printf("#%d %.4f\n", tc, sub_score + (double)sub_time / CLOCKS_PER_SEC / 10);
		total_score += sub_score;
		total_time += sub_time;

	}
	printf("SCORE :: %f\n", total_score + (double)total_time / CLOCKS_PER_SEC / 10);
}
