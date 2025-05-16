#include <stdio.h>
#define MAP_SIZE 10'000		// 10'000
#define SQUARE_NUM 10'000		// 10'000

namespace debug {
	void printSquare(int sx, int sy, int ex, int ey, char map[MAP_SIZE][MAP_SIZE]) {
		for (int j = ey; j >= sy; j--) {
			for (int i = sx; i <= ex; i++) {
				if (map[i][j] == 0) printf("- ");
				else if (map[i][j] == 1) printf("* ");
				else printf("O ");
			}
			printf("\n");
		}
	}
	void printMap(char map[MAP_SIZE][MAP_SIZE]) {
		printf("[MAP]\n");
		printSquare(0, 0, MAP_SIZE - 1, MAP_SIZE - 1, map);
	}
};

int line_num;

void storeLine(unsigned int x, unsigned int y, unsigned int z, char data[150000], unsigned int& idx, unsigned int dir) {
	data[idx++] = x & 0xFF;
	data[idx++] = (x & 0xFF00) >> 8;
	data[idx++] = y & 0xFF;
	data[idx++] = (y & 0xFF00) >> 8;
	data[idx++] = z & 0xFF;
	data[idx++] = (z & 0xFF00) >> 8 | dir << 7;
	line_num++;
}
void restoreLine(char data[150000], unsigned int& idx, unsigned int& x, unsigned int& y, unsigned int& z, unsigned int& dir) {
	unsigned int data0, data1;
	data0 = data[idx++] & 0xFF;
	data1 = data[idx++] & 0xFF;
	x = (data1 << 8) + data0;
	data0 = data[idx++] & 0xFF;
	data1 = data[idx++] & 0xFF;
	y = (data1 << 8) + data0;
	data0 = data[idx++] & 0xFF;
	data1 = data[idx++] & 0xFF;
	z = ((data1 << 8) + data0) & 0x7FFF;
	dir = (((data1 << 8) + data0) & 0x8000) >> 15;		// 0: x-axis, 1: y-axis
}

void comp(char map[MAP_SIZE][MAP_SIZE], char data1[150000]) {
	unsigned int sx, sy, ex, ey, data_idx = 0;
	line_num = 0;
	for (sx = 0; sx < MAP_SIZE; sx++) {
		for (sy = 0; sy < MAP_SIZE; sy++) {
			if (map[sx][sy] == 0 || map[sx][sy] == 2) continue;

			for (ex = sx + 1; ex < MAP_SIZE; ex++) {
				if (map[ex][sy] == 0) break;
			}
			for (ey = sy + 1; ey < MAP_SIZE; ey++) {
				if (map[sx][ey] == 0) break;
			}
			ex--; ey--;
			if (ex - sx >= ey - sy) {
				storeLine(sx, sy, ex, data1, data_idx, 0);
				for (unsigned int z = sx; z <= ex; z++) map[z][sy] = 2;
			}
			else {
				storeLine(sx, sy, ey, data1, data_idx, 1);
				for (unsigned int z = sy; z <= ey; z++) map[sx][z] = 2;
			}

			if (data_idx >= 150000) {
				printf("data overload\n");
				break;
			}
		}
		if (data_idx >= 150000) break;
	}
}

void decomp(char map[MAP_SIZE][MAP_SIZE], char data2[150000]) {
	for (int i = 0; i < MAP_SIZE; ++i) {
		for (int j = 0; j < MAP_SIZE; ++j) {
			map[i][j] = 0;
		}
	}

	unsigned int sx, sy, z, data_idx = 0, dir;
	for (int n = 0; n < line_num; n++) {
		restoreLine(data2, data_idx, sx, sy, z, dir);
		if (dir == 0) for (unsigned int i = sx; i <= z; i++) map[i][sy] = 1;
		if (dir == 1) for (unsigned int i = sy; i <= z; i++) map[sx][i] = 1;
	}
}

/*
* [직선의 양 끝을 저장하겠다]
* seed: 5 -> score: 2996
*/
