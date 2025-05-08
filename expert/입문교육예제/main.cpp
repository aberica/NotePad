#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#define TCASE 2048

static int org[TCASE][10000];

inline int erand() {
	if (RAND_MAX <= 0x7fff) {
		return rand() * (RAND_MAX + 1) + rand();
	}
	else return rand();
}

extern void test(int data[][10000]);

static int data[TCASE][10000];

int main() {
	srand(55);
	setbuf(stdout, NULL);
	int N, M;
	for (register int tcase = 0; tcase < TCASE; ++tcase) {
		N = erand() % 50 + 1;
		M = (erand() % 1000 + 1) * (erand() % 2 ? 1 : -1);

		for (register int i = 0; i < 10000; ++i) {
			org[tcase][i] = (erand() % N + 1) * (erand() % 2 ? 1 : -1);
			data[tcase][i] = org[tcase][i] + M * (i % 2 ? 1 : -1);
		}
	}

	clock_t c = clock();
	test(data);
	c = clock() - c;

	int SCORE = 0;
	for (register int tcase = 0; tcase < TCASE; ++tcase) {
		if (!memcmp(org[tcase], data[tcase], sizeof(int) * 10000)) SCORE += 10000;
	}
	printf("SCORE : %d\n", SCORE + 10000 - (int)c);
	return 0;
}
