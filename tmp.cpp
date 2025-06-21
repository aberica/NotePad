#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

extern void init(int N, int K, int mId[], int sCity[], int eCity[], int mToll[]);
extern void add(int mId, int sCity, int eCity, int mToll);
extern void remove(int mId);
extern int calculate(int sCity, int eCity);

/////////////////////////////////////////////////////////////////////////

#define MAX_K 5000
#define CMD_INIT 100
#define CMD_ADD 200
#define CMD_REMOVE 300
#define CMD_CALC 400

static bool run() {
	int q;
	scanf("%d", &q);

	int n, k;
	int mIdArr[MAX_K], sCityArr[MAX_K], eCityArr[MAX_K], mTimeArr[MAX_K];
	int mId, sCity, eCity, mTime;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
			case CMD_INIT:
				okay = true;
				scanf("%d %d", &n, &k);
				for (int j = 0; j < k; ++j) {
					scanf("%d %d %d %d", &mIdArr[j], &sCityArr[j], &eCityArr[j], &mTimeArr[j]);
				}
				init(n, k, mIdArr, sCityArr, eCityArr, mTimeArr);
				break;
			case CMD_ADD:
				scanf("%d %d %d %d", &mId, &sCity, &eCity, &mTime);
				add(mId, sCity, eCity, mTime);
				break;
			case CMD_REMOVE:
				scanf("%d", &mId);
				remove(mId);
				break;
			case CMD_CALC:
				scanf("%d %d %d", &sCity, &eCity, &ans);
				ret = calculate(sCity, eCity);
				if (ans != ret)
					okay = false;
				break;
			default:
				okay = false;
				break;
		}
	}
	return okay;
}

int main() {
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////
//    제출 시 이 윗 부분은 변경없이 그대로 제출해야 합니다. 
/////////////////////////////////////////////////////////////////////////

void init(int N, int K, int mId[], int sCity[], int eCity[], int mTime[]) {
	return;
}

void add(int mId, int sCity, int eCity, int mTime) {
	return;
}

void remove(int mId) {
	return;
}

int calculate(int sCity, int eCity) {
	return 0;
}
