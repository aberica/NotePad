#include <stdio.h>

#define FRONT 0
#define BACK 1
#define UP 2
#define DOWN 3
#define LEFT 4
#define RIGHT 5

//int cube_a[6][3][3], cube_b[6][3][3], cube_tmp[6][3][3];
int cube_a_list[250'000][6][3][3], cube_b_list[250'000][6][3][3];
int candi[250'000][2], candi_size = 0;

extern void rotate(int face, bool clockwise);
namespace sol {
	void rotate(int Cube[6][3][3], int face, bool clockwise)
	{
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
};
void copyCube(int from[6][3][3], int to[6][3][3]) {
	register int s, i, j;
	for (s = 0; s < 6; s++)
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				to[s][i][j] = from[s][i][j];
}
bool matchCube(int cube_a[6][3][3], int cube_b[6][3][3]) {
	register int s, i, j;
	for (s = 0; s < 6; s++)
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				if (cube_a[s][i][j] != cube_b[s][i][j]) return false;
	return true;
}
void cubeInit(int arr[6][3][3]) {
	register int s, i, j;
	for (s = 0; s < 6; s++)
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				cube_a_list[0][s][i][j] = s;
	copyCube(cube_b_list[0], arr);
}
int pow(int n, int t) {
	int output = 1;
	for (int i = 0; i < t; i++) output *= n;
	return output;
}

void flattenCube(int from[6][3][3], int to[54]) {
	for (int i = 0; i < 6; i++) 
		for (int j = 0; j < 3; j++) 
			for (int k = 0; k < 3; k++) 
				to[i * 9 + j * 3 + k] = from[i][j][k];
}
void swapCube(int A[6][3][3], int B[6][3][3]) {
	int tmp[6][3][3];
	copyCube(A, tmp);
	copyCube(B, A);
	copyCube(tmp, B);
}
bool smaller(int is[54], int than[54]) {
	for (int i = 0; i < 54; i++) {
		if (is[i] < than[i]) return true;
		else if (is[i] == than[i]) continue;
		else return false;
	}
	return false;
}
bool smaller(int _is[6][3][3], int _than[6][3][3]) {
	int is[54], than[54];
	flattenCube(_is, is);
	flattenCube(_than, than);
	return smaller(is, than);
}
bool same(int _is[6][3][3], int _than[6][3][3]) {
	int is[54], than[54];
	flattenCube(_is, is);
	flattenCube(_than, than);
	for (int i = 0; i < 54; i++) {
		if (is[i] != than[i]) return false;
	}
	return true;
}
int medianOfThree(int arr[250'000][6][3][3], int low, int mid, int high) {
	int a[54], b[54], c[54];
	flattenCube(arr[low], a);
	flattenCube(arr[mid], b);
	flattenCube(arr[high], c);

	if (smaller(a, b) && smaller(b, c)) return mid;
	else if (smaller(b, a) && smaller(a, c)) return low;
	else return high;
}
int partition(int arr[250'000][6][3][3], int low, int high) {
	int mid = low + (high - low) / 2;
	int pivot_index = medianOfThree(arr, low, mid, high);
	swapCube(arr[pivot_index], arr[high]);
	int pivot[6][3][3];
	copyCube(pivot, arr[high]);

	int i = low - 1;
	for (int j = low; j < high; j++) {
		if (smaller(arr[j], pivot)) continue;
		swapCube(arr[++i], arr[j]);
	}

	swapCube(arr[++i], arr[high]);
	return i;
}
void quickSort(int arr[250'000][6][3][3], int low, int high) {
	printf("  sort - %d ~ %d\n", low, high);
	if (low < high) {
		int pt = partition(arr, low, high);

		quickSort(arr, low, pt - 1);
		quickSort(arr, pt + 1, high);
	}
}

void runtest(int arr[6][3][3]) {
	cubeInit(arr);
	for (int i = 0, sz = 1; i < 5; i++, sz *= 12) {
		for (int j = sz - 1; j >= 0; j--) {
			for (int k = 0; k < 12; k++) {
				copyCube(cube_a_list[j], cube_a_list[j * 12 + k]);
				sol::rotate(cube_a_list[j * 12 + k], k / 2, k % 2);
				copyCube(cube_b_list[j], cube_b_list[j * 12 + k]);
				sol::rotate(cube_b_list[j * 12 + k], k / 2, k % 2);
			}
		}
	}

	int cube_list_size = pow(12, 5);
	quickSort(cube_a_list, 0, cube_list_size - 1);
	printf("t1\n");
	quickSort(cube_b_list, 0, cube_list_size - 1);
	printf("t2\n");

	for (int i = 0, j = 0; i < cube_list_size && j < cube_list_size; i++) {
		while (smaller(cube_b_list[j], cube_a_list[i])) j++;
		if (same(cube_a_list[i], cube_b_list[j])) {
			candi[candi_size][0] = i;
			candi[candi_size++][1] = j;

			j++;
			break;		// debug
		}
	}

	int act[10] = { 0, };
	for (int i = 0; i < candi_size; i++) {
		for (int j = 4; j >= 0; j--) {
			act[j] = candi[i][0] % 12;
			candi[i][0] /= 12;
		}
		for (int j = 5; j < 10; j++) {
			act[j] = candi[i][1] % 12;
			candi[i][1] /= 12;
		}
	}
	for (int i = 0; i < 10; i++) {
		rotate(act[i] / 2, act[i] % 2);
	}
}
