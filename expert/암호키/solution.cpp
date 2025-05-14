#include <stdio.h>
#define KEY_SIZE 200

unsigned char answer[KEY_SIZE];
int visited[KEY_SIZE];				// 0: not visited, 2: pos, 1: equal, -1: No

void init() {
	for (register int i = 0; i < KEY_SIZE; i++) {
		answer[i] = 0;
		visited[i] = 0;
	}
}

extern int check(unsigned char key[KEY_SIZE]);
void test(unsigned char key[KEY_SIZE]) {
	init();

	int n = 2;
	int candi[KEY_SIZE], ed = 0;
	while()

	//for (register int i = 0; i < KEY_SIZE; i++) {
	//	for (register int j = 0; j < KEY_SIZE; j++) key[j] = 0;

	//	for (register int j = 1; j < 256; j++) {
	//		if (visited[j] == -1 || visited[j] == 2) continue;
	//		key[i] = j;
	//		int check_value = check(key);
	//		if (check_value == 257) {		// 256 + 1
	//			visited[j] = 2;
	//			answer[i] = j;
	//			break;
	//		}
	//		else if (check_value >= 1) visited[j] = 1;
	//		else visited[j] = -1;
	//	}
	//}

	for (register int i = 0; i < KEY_SIZE; i++) {
		key[i] = answer[i];
	}
}

/*
* [SCORE]
* ���� - seed: 3, tc: 1
* ��������� - 1659
* �湮���� - 606
* n�� �������� - 
*/
