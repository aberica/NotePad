#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>

#define MAXL (10)

const int CMD_INIT = 100;
const int CMD_ADD = 200;
const int CMD_REMOVE = 300;
const int CMD_FINDWORD = 400;
const int CMD_FINDPAGE = 500;

extern void init(int N, char mWordList[][MAXL + 1], int mWordSize);
extern void addWord(char mWordList[][MAXL + 1], int mWordSize);
extern void removeWord(char mWordList[][MAXL + 1], int mWordSize);
extern char* findWord(int mPageNum);
extern int findPage(char mWord[]);

char mWordList[10000][MAXL + 1];

static bool run()
{
	bool okay = false;
	int Q, N, mWordSize, mPageNum, userRet;
	char mAns[MAXL + 1], mWord[MAXL + 1], * userAns;
	scanf("%d", &Q);

	for (int q = 0; q < Q; ++q)
	{
		int cmd;
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d%d", &N, &mWordSize);
			for (int i = 0; i < mWordSize; i++)
				scanf("%s", mWordList[i]);

			init(N, mWordList, mWordSize);
			okay = true;
			break;
		case CMD_ADD:
			scanf("%d", &mWordSize);
			for (int i = 0; i < mWordSize; i++)
				scanf("%s", mWordList[i]);

			addWord(mWordList, mWordSize);
			break;
		case CMD_REMOVE:
			scanf("%d", &mWordSize);
			for (int i = 0; i < mWordSize; i++)
				scanf("%s", mWordList[i]);

			removeWord(mWordList, mWordSize);
			break;
		case CMD_FINDWORD:
			scanf("%d", &mPageNum);

			userAns = findWord(mPageNum);

			scanf("%s", mAns);
			if (strcmp(userAns, mAns) != 0)
				okay = false;

			break;
		case CMD_FINDPAGE:
			scanf("%s", mWord);

			userRet = findPage(mWord);

			scanf("%d", &mPageNum);
			if (userRet != mPageNum)
				okay = false;

			break;
		default:
			okay = false;
			break;
		}
	}

	return okay;
}

int main()
{
	setbuf(stdout, NULL);
	//	freopen("sample_input.txt", "r", stdin);

	int TC, MARK;

	scanf("%d %d", &TC, &MARK);
	for (int tc = 1; tc <= TC; ++tc)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////
//    제출 시 이 윗 부분은 변경없이 그대로 제출해야 합니다. 
/////////////////////////////////////////////////////////////////////////

void init(int N, char mWordList[][MAXL+1], int mWordSize)
{

}

void addWord(char mWordList[][MAXL+1], int mWordSize)
{

}

void removeWord(char mWordList[][MAXL + 1], int mWordSize)
{

}

char mRet[MAXL + 1];
char* findWord(int mPageNum)
{
	return mRet;
}

int findPage(char mWord[])
{
	return 0;
}
