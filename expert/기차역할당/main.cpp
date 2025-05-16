#include <stdio.h>
#include <math.h>
#include <time.h>
#define HOUSE_NUM 100'000
#define STATION_NUM 5
#define MAX_ALLOC 22'000

extern void test(const double unsorted_house[], double station[], short alloc[]);

unsigned short randint(void) {
	static unsigned long long seed = 5;
	seed *= 2541684013ULL;
	return seed >> 17;
}

double house[HOUSE_NUM];
double station[STATION_NUM];
short alloc[HOUSE_NUM];

void build() {
	int alloc_cnt = 0;
	int alpha = randint() % 100;
	int beta = randint() % 100;
	while (alloc_cnt < HOUSE_NUM) {
		int tmp = 0;
		for (int c = 0; c < 3; ++c) tmp += randint() % 100;
		house[alloc_cnt++] = (double)((tmp / 3 + alpha) % 100) + (randint() % 10000) / 10000.0;
		house[alloc_cnt++] = (double)((tmp / 3 + beta) % 100) + (randint() % 10000) / 10000.0;
	}
	return;
}

namespace debug {
	void AnalyzeHouse(int n = 10) {
		for (int i = 0; i < 10; i++) {
			printf("house[%d] : %lf\n", i, house[i]);
		}
		double mn = 1000000, mx = -1000000;
		for (int i = 0; i < HOUSE_NUM; i++) {
			if (house[i] > mx) mx = house[i];
			if (house[i] < mn) mn = house[i];
		}
		printf("mn : %lf, mx : %lf\n", mn, mx);		// seed: 5 - mn: 0.000300, mx: 99.999500
	}
	void Experiment() {
		short a;
		printf("%zd", sizeof(a));
	}
}

int main() {
	build();
	//debug::AnalyzeHouse();
	double SCORE = 0.0;
	double c = clock();
	test(house, station, alloc);
	for (int i = 0; i < HOUSE_NUM; ++i) {
		SCORE += fabs(house[i] - station[alloc[i]]);
	}
	int check[STATION_NUM] = { 0, };
	for (int i = 0; i < HOUSE_NUM; ++i) ++check[alloc[i] % 5];
	for (int i = 0; i < STATION_NUM; ++i) if (check[i] > MAX_ALLOC) {
		printf("Penalty Occured\n");
		SCORE += 100'000'000.0;
		break;
	}
	SCORE += (clock() - c) / 1000.0;
	printf("SCORE : %lf\n", SCORE);
	return 0;
}
