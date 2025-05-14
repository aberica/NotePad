// [��ȣŰ]
// http://swtest.sds.samsung.net/common/practice/problem/view.do?problemId=AWvBUXMQOnGojUGh
#include <stdio.h>
#include <time.h>
#define KEY_SIZE 200	// 200
#define TC 1			// 50

static unsigned char dummy[2560];
static unsigned char KEY[KEY_SIZE];
static unsigned char dummy2[2560];
static unsigned int seed = 3;
static unsigned char dummy3[2560];
extern void test(unsigned char key[KEY_SIZE]);

int rand() {
	seed = seed * 214011 + 2531011;
	return (seed >> 16) & 0x7FFF;
}

int count;
int check(unsigned char key[KEY_SIZE]) {
	count++;
	int equal = 0, pos = 0;
	for (register int i = 0; i < KEY_SIZE; i++) {
		if (key[i] == KEY[i]) {
			pos += 1;
		}
	}
	for (register int i = 0; i < KEY_SIZE; i++) {
		for (register int j = 0; j < KEY_SIZE; j++) {
			if (key[i] == KEY[j]) {
				equal += 1;
			}
		}
	}
	return pos * 256 + equal;
}

void makeKey() {
	unsigned int pool[256] = { 0 };
	int idx;
	for (register int i = 0; i < KEY_SIZE; i++) {
		KEY[i] = 0;
	}
	for (register int i = 0; i < KEY_SIZE; i++) {
		idx = rand() % 255;
		while (pool[idx] != 0) {
			idx = rand() % 255;
		}
		KEY[i] = idx + 1;
		pool[idx] = 1;
	}
}

namespace debug {
	void printKEY(int n) {
		printf("[Print KEY]\n");
		unsigned char mx = 0, mn = 0xFF;
		bool visited[256] = { false, };
		for (int i = 0; i < n; i++) {
			printf("%u ", KEY[i]);
			if (KEY[i] > mx) mx = KEY[i];
			if (KEY[i] < mn) mn = KEY[i];
			if (visited[KEY[i]]) printf("duplicated exist");
			visited[KEY[i]] = true;
		}
		printf("\n");
		printf("mn: %u, mx: %u\n", mn, mx);

		// seed3 - �ߺ� ����, �ּ� 1 �ִ� 231
	}
}

int main() {
	int PENALTY = 0;
	for (register int i = 0; i < TC; i++) {
		count = 0;
		unsigned char key[KEY_SIZE] = { 0 };
		makeKey();
		clock_t t0 = clock();
		test(key);
		PENALTY += (clock() - t0) / (CLOCKS_PER_SEC / 1000);
		for (register int j = 0; j < KEY_SIZE; j++) {
			if (key[j] != KEY[j]) {
				PENALTY += 10000;

				// debug
				printf("Penalty Occured\n");
				break;
			}
		}
	}
	printf("PENALTY: %d\n", PENALTY);
	return 1;
}
