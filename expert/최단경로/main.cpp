#include <stdio.h>
#include <math.h>
#include <time.h>
#define MAP_SIZE 40'000

static unsigned long long seed = 13;

static int result = 80275;

static char map[MAP_SIZE][MAP_SIZE];

extern int test(char map[][MAP_SIZE]);

static void build() {
	for (register int y = 0; y < MAP_SIZE; ++y) {
		for (register int x = 0; x < MAP_SIZE; ++x) {
			seed = seed * 25214903917ULL;
			for (register int c = 0; c < 64; c += 2) {
				map[y][x++] = (seed >> c) & 0x000000000000003ULL;
			}
		}
	}
}

bool DEBUG = true;
namespace debug {
	void printMap(int x_max, int y_max) {
		for (int y = 0; y < y_max; y++) {
			for (int x = 0; x < x_max; x++) {
				printf("%d ", map[y][x]);
			}
			printf("\n");
		}
	}
	void printMinMax() {
		int mn = 10'000'000, mx = -1;
		for (register int y = 0; y < MAP_SIZE; ++y) {
			for (register int x = 0; x < MAP_SIZE; ++x) {
				if (mn > map[y][x]) mn = map[y][x];
				if (mx < map[y][x]) mx = map[y][x];
			}
		}
		printf("min: %d, max: %d\n", mn, mx);
	}
	void printCharMax() {
		printf("%d\n", 0xFF);	// 255
	}
}

int main() {
	double SCORE = 0.;
	build();
	if (DEBUG) {
		debug::printMap(20, 20);
		//debug::printMinMax();
		//debug::printCharMax();
	}
	time_t time = clock();
	if (test(map) != result) SCORE += 100'000'000;
	SCORE += (double)(clock() - time) / CLOCKS_PER_SEC;
	printf("SCORE : %lf\n", SCORE);
}
