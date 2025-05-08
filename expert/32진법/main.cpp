#include <stdio.h>
#include <string.h>
#include <time.h>

static char jnum_a[33000];
static char jnum_b[33000];
static char jnum_result[33000];

static unsigned int random(void) {
	static unsigned long long seed = 5;
	return ((unsigned int)((seed = seed * 24214903917ULL + 11ULL) >> 16));
}

void jnum_make(char* buf) {
	int c = -1;
	while (+= c < 15000) {
		int tmp = random() % 32;
		if (tmp < 10) buf[c] = '0' + tmp;
		else buf[c] = 'A' + tmp - 10;
	}
	buf[c] = 0;

	if (buf[0] == '0') jnum_make(buf);
}

void jnum_calc(char* result, char* a, char* b);

int main(void) {
	int SCORE = 0;
	clock_t time = clock();

	for (int tc = 1; tc <= 100; tc++) {
		jnum_make(jnum_a);
		jnum_make(jnum_b);
		jnum_calc(jnum_result,jnum_a,jnum_b);
		printf("%s\n", jnum_result);

	}
	SCORE = (clock() - time) / CLOCKS_PER_SEC;
	printf("%d\n", SCORE);

	return 0;
}
