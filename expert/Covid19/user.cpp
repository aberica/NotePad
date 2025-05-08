#include <stdio.h>
#define MAX_TREE_HEIGHT 30

static const int MAX_SAMPLE = 50'000'000;
extern int covid_checkup(int n);
extern void culture(int n, int a, int b);
extern void clear(int n);

int child_num[MAX_TREE_HEIGHT] = { 0, };
int tree_st[MAX_TREE_HEIGHT] = { 0, };
int tree_height= 0;

bool DEBUG = false;
namespace debug {
	double calProb(double prob, int n) {
		double output = 1.0;
		for (int i = 0; i < n; i++) output *= (1 - prob);
		return output;
	}
	void printTreeInfo() {
		for (int i = 0; i <= tree_height; i++) {
			printf("%d - child_num: %d, st: %d\n", i, child_num[i], tree_st[i]);
		}
	}
}

void init() {
	// Tree child 개수 지정
	int st = 3;
	int descent = 9;
	for (int i = 0; i < MAX_TREE_HEIGHT; i++) {
		child_num[i] = st;
		if (i % descent == descent - 1 && st != 2) st--;
	}

	// child_num에 맞추어 tree의 시작점 지정
	tree_st[0] = 0;
	for (int i = 0, sample_num = MAX_SAMPLE; \
		i < MAX_TREE_HEIGHT - 1 && sample_num > 1; \
		i++, sample_num /= child_num[i]) {

		tree_st[i + 1] = tree_st[i] + sample_num - sample_num % child_num[i];
		tree_height = i + 1;
	}

	if(DEBUG) debug::printTreeInfo();
}
void cureTree(int idx, int th);
void makeTree() {
	for (int th = 0, sample_num = MAX_SAMPLE; \
		th < MAX_TREE_HEIGHT - 1 && sample_num > 1; \
		th++, sample_num /= child_num[th]) {

		for (int from = tree_st[th], to = tree_st[th + 1]; \
			from < tree_st[th + 1]; \
			from += child_num[th], to++) {

			for (int j = 0; j < child_num[th]; j++) culture(to, to, from + j);
		}
		for (int from = tree_st[th + 1]; from < tree_st[th] + sample_num; from++) {
			cureTree(from, th);
		}
	}
}
void cureTree(int idx, int th) {	//tree_height
	if (!covid_checkup(idx)) return;
	if (th == 0) clear(idx);
	else {
		int from = tree_st[th - 1] + (idx - tree_st[th]) * child_num[th - 1];
		for (int i = 0; i < child_num[th - 1]; i++) {
			cureTree(from + i, th - 1);
		}
	}
}

void test(void) {
	init();
	makeTree();
	cureTree(tree_st[tree_height], tree_height);
}

/*
* [n진 트리] - 곁다리 버리기
* st: child 개수 시작 / dc: 감소주기
* 
* seed: 5
* 1506118.007 - st = 2, dc = .
* 1489125.006 - st = 3, dc = 2
* 1461169.005 - st = 3, dc = 5
* 1449088.005 - st = 3, dc = 6
* 1436030.005 - st = 3, dc = 7
* 1428665.005 - st = 3, dc = 8
* 1426095.005 - st = 3, dc = 9		<< 최소
* 1425253.005 - st = 3, dc = 10
* 1487216.005 - st = 4, dc = 2
* 1469299.004 - st = 4, dc = 3
* 1464009.005 - st = 4, dc = 4
* 1462915.004 - st = 4, dc = 5
* 1458180.004 - st = 4, dc = 6
* 1741431.005 - st = 4, dc = 7
* 1505300.004 - st = 5, dc = 2
* 
* seed: 7
* 1508118.007 - st = 2, dc = .
* 1450687.005 - st = 3, dc = 6
* 1430303.005 - st = 3, dc = 8
* 1427733.005 - st = 3, dc = 9		<< 최소는 아니나 얼추 비슷
* 1426891.005 - st = 3, dc = 10
* 1426609.005 - st = 3, dc = 11
* 1426512.005 - st = 3, dc = 12
* 1465098.005 - st = 4, dc = 5
* 1460328.005 - st = 4, dc = 6
* 1747799.005 - st = 4, dc = 7
* 2485580.005 - st = 5, dc = 6
*/
