#include <stdio.h>
#include <time.h>

void test(void);

static const int MAX_SAMPLE = 50000000;
static double SCORE = 0.0;
static int tray[2 * MAX_SAMPLE];

static unsigned int rand(void) {
	static unsigned long long seed = 5;
	seed = seed * 25214903917ULL + 11ULL;
	return (unsigned int)(seed >> 16);
}

int covid_checkup(int n) {
	SCORE += 1.0;
	if (n < 0 || n >= 2 * MAX_SAMPLE) return 0;
	if (tray[n] == 0) return 0;
	return 1;
}

void culture(int n, int a, int b) {
	if (n < MAX_SAMPLE || n >= 2 * MAX_SAMPLE) return;
	tray[n] = tray[a] | tray[b];
}

void clear(int n) {
	SCORE += 10.0;
	if (n < 0 || n >= 2 * MAX_SAMPLE) return;
	tray[n] = 0;
}

extern bool DEBUG;
int main(void) {
	for (register int c = 0; c < 2 * MAX_SAMPLE; c++) tray[c] = 0;
	for (register int c = 0; c < MAX_SAMPLE / 1000; c++) tray[rand() % MAX_SAMPLE] = 1;

	clock_t time = clock();
	test();
	SCORE += (double)(clock() - time) / CLOCKS_PER_SEC / 100;

	for (register int c = 0; c < MAX_SAMPLE; c++) {
		if (tray[c]) {
			SCORE += 10000.0;
			if (DEBUG) printf("not cured\n");		// DEBUG
			if (DEBUG) break;						// DEBUG
		}
	}
	printf("SCORE : %.3f\n", SCORE);
	return 0;

}
