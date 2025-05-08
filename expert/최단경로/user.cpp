#include <stdio.h>
#define MAP_SIZE 40'000
#define QUEUE_SIZE 25'000'000
extern bool DEBUG;

typedef struct {
	int y, x;
	int score;
} Node;

int pos_change[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
Node que[QUEUE_SIZE];
int st, ed;

bool coordAvail(int y, int x) {
	return 0 <= y && y < MAP_SIZE && 0 <= x && x < MAP_SIZE;
}
int getDiff(char a, char b) {
	int output = (a & 0x7F) - (b & 0x7F);
	return output < 0 ? -output : output;
}
void BFS(char map[][MAP_SIZE], Node node) {
	map[node.y][node.x] |= 0x80;

	for (int i = 0; i < 4; i++) {
		int nxt_y = node.y + pos_change[i][0], nxt_x = node.x + pos_change[i][1];
		if (!coordAvail(nxt_y, nxt_x)) continue;
		if (map[nxt_y][nxt_x] & 0x80) continue;		// visited
		if (getDiff(map[node.y][node.x], map[nxt_y][nxt_x]) > 1) continue;

		que[ed++] = { nxt_y, nxt_x, node.score + 1 };
		if (ed == QUEUE_SIZE) ed = 0;
		if (st == ed) {
			printf("QUEUE SIZE IS SMALL !!!\n");
			break;
		}
	}
}

int test(char map[][MAP_SIZE]) {
	st = 0, ed = 1;
	que[st] = { 0, 0, 0 };
	while (st != ed) {
		if (que[st].x == MAP_SIZE - 1 && que[st].y == MAP_SIZE - 1) return que[st].score;

		//if (DEBUG) printf("map[%d][%d] - %d\n", que[st].y, que[st].x, que[st].score);
		//if (DEBUG && que[st].y > 100) break;
		BFS(map, que[st++]);
		if (st == QUEUE_SIZE) st = 0;
	}

	return 0;
}

/*
* [DFS]
*   ȯ
*/
