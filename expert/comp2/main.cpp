#include <stdio.h>
#include <time.h>

char map[10000][10000];
char map_org[10000][10000];
char result[10000][10000];
char data[150000];

extern void comp(char map[10000][10000], char data1[150000]);
extern void decomp(char map[10000][10000], char data2[150000]);

static unsigned int random(void) {
	static unsigned long long seed = 70;
	return ((unsigned int)((seed = seed * 25214903917ULL + 11ULL) >> 16));
}

void build(void) {
	for (int i = 0; i <= 9999; ++i) {
		for (int j = 0; j <= 9999; ++j) {
			map[i][j] = map_org[i][j] = 0;
		}
	}
	int sx, ex, sy, ey;
	for (int c = 0; c < 10000; ++c) {
		sx = random() % 10000;
		sy = random() % 10000;
		ex = random() % 10000;
		ey = random() % 10000;
		if (ex <= sx || ey <= sy) {
			--c;
			continue;
		}
		for (int x = sx; x <= ex; ++x) {
			map[x][sy] = map_org[x][sy] = 1;
			map[x][ey] = map_org[x][ey] = 1;
		}
		for (int y = sy+1; y < ey; ++y) {
			map[sx][y] = map_org[sx][y] = 1;
			map[ex][y] = map_org[ex][y] = 1;
		}
	}
}

bool verify(char map[10000][1000], char result[10000][10000]) {
	for (int i = 0; i <= 9999; ++i)
		for (int j = 0; j <= 9999; ++j)
			if (map[i][j] != result[i][j])
				return false;
	return true;
}

int main() {
	int score = 0;
	build();
	double c = clock();
	comp(map, data);
	decomp(result, data);
	score += (int)(clock() - c);
	if (!verify(map_org, result)) score += 10000000;
	return 0;
}
