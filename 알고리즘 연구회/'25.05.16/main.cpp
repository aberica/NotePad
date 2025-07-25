/*
제목 : 손상된 도로
시험날짜 : 2025년 07월 19일
시간제한 : 테스트 케이스당 1초
메모리제한 : total 256MB (단, stack 1MB)

<제약조건>
1. User Code 내 어떠한 헤더 파일도 추가할 수 없다. (단, malloc.h는 사용 가능)
2. main은 수정되지 않으며, 채점 시에도 그대로 사용된다. (단, seed는 변경될 수 있다.)
3. 본 검정은 대단히 엄격한 코드리뷰를 수행한다.
4. 본 검정은 C++만 지원한다.
5. 실제 채점은 테스트 케이스 100개 이내에서 수행된다.


SCORE 값이 낮을 수록 높은 점수가 부여된다.
================================================================
합격 점수
1130만점 내외로 예상
================================================================
written by pirateT9
*/
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

static unsigned long long seed;
static int pseudo_rand(void)
{
	seed = seed * 25214903917ULL + 11ULL;
	return (seed >> 16) & 0x3fffffff;
}

static const int MAX_CITY = 1000;
static const int MAX_FREIGHT = 2000;
static const int MAX_ROAD = 3000;
static const int MAX_WEIGHT = 90;
static const long long PENALTY = 1000000000000;
static const int ShuffleCnt = 3000;

static long long gTotalScore;

struct ROAD {
	int city1, city2, status, roadid;
};

struct FREIGHT {
	int pos, dest, weight;
};

struct TRUCK {
	int pos;
	FREIGHT* FreightPT;
};

static ROAD Road[MAX_ROAD];
static FREIGHT Freight[MAX_FREIGHT];
static int City[MAX_CITY];
TRUCK Truck;

void getroad(ROAD road[])
{
	for (int i = 0; i < MAX_ROAD; i++)
		road[i] = Road[i];
}

void getfreight(FREIGHT freight[])
{
	for (int i = 0; i < MAX_FREIGHT; i++)
		freight[i] = Freight[i];
}

bool move(int RoadID, int repair)
{
	if (RoadID < 0 || RoadID >= MAX_ROAD || repair < 0 || Road[RoadID].status + repair > 1000) return false;
	if (Truck.pos != Road[RoadID].city1 && Truck.pos != Road[RoadID].city2) return false;

	int TotalWeight = 10 + (Truck.FreightPT == nullptr ? 0 : Truck.FreightPT->weight);

	if (repair + Road[RoadID].status >= TotalWeight) {
		Road[RoadID].status += repair - TotalWeight;
		gTotalScore += TotalWeight * repair + 10;

		if (Truck.pos == Road[RoadID].city1)
			Truck.pos = Road[RoadID].city2;
		else
			Truck.pos = Road[RoadID].city1;

		return true;
	}
	else
		return false;
}

bool load(int FreightID)
{
	if (FreightID < 0 || FreightID >= MAX_FREIGHT) return false;

	if (Truck.FreightPT != nullptr || Freight[FreightID].pos != Truck.pos) return false;
	gTotalScore += Freight[FreightID].weight * Freight[FreightID].weight;
	Truck.FreightPT = &Freight[FreightID];

	return true;
}

bool unload()
{
	if (Truck.FreightPT == nullptr) return false;
	gTotalScore += Truck.FreightPT->weight * Truck.FreightPT->weight;
	Truck.FreightPT->pos = Truck.pos;
	Truck.FreightPT = nullptr;

	return true;
}

static void init()
{
	int i, j;

	Truck.pos = 0;
	Truck.FreightPT = nullptr;

	for (i = 0; i < MAX_CITY; i++)
		City[i] = i;

	int roadcnt = 0;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < ShuffleCnt; j++) {
			int x = pseudo_rand() % MAX_CITY, y = pseudo_rand() % MAX_CITY;
			int temp = City[x];
			City[x] = City[y];
			City[y] = temp;
		}
		for (j = 0; j < MAX_CITY - 1; j++) {
			Road[roadcnt].city1 = City[j];
			Road[roadcnt].city2 = City[j + 1];
			Road[roadcnt].status = pseudo_rand() % 300 + 100;
			Road[roadcnt++].roadid = roadcnt;
		}
	}

	while (roadcnt < MAX_ROAD) {
		for (j = 0; j < ShuffleCnt; j++) {
			int x = pseudo_rand() % MAX_CITY, y = pseudo_rand() % MAX_CITY;
			int temp = City[x];
			City[x] = City[y];
			City[y] = temp;
		}

		int AddRoad = pseudo_rand() % MAX_CITY;

		if (AddRoad + roadcnt > MAX_ROAD)
			AddRoad = MAX_ROAD - roadcnt;

		int start = pseudo_rand() % (MAX_CITY - 1);
		if (start + AddRoad > MAX_CITY - 1)
			AddRoad = MAX_CITY - start - 1;

		for (i = 0; i < AddRoad; i++) {
			Road[roadcnt].city1 = City[start + i];
			Road[roadcnt].city2 = City[start + i + 1];
			Road[roadcnt].status = pseudo_rand() % 300 + 100;
			Road[roadcnt++].roadid = roadcnt;
		}
	}

	for (i = 0; i < MAX_FREIGHT; i++) {
		Freight[i].pos = pseudo_rand() % MAX_CITY;
		Freight[i].dest = pseudo_rand() % MAX_CITY;
		Freight[i].weight = pseudo_rand() % MAX_WEIGHT + 1;
	}
}

static bool verify()
{
	for (int i = 0; i < MAX_FREIGHT; i++)
		if (Freight[i].pos != Freight[i].dest)
			return false;

	return true;
}

extern void process();

int main()
{
	int T;
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin) ;
	scanf("%d %lld", &T, &seed);
	clock_t begin = clock();

	gTotalScore = 0;

	for (int tc = 0; tc < T; tc++) {
		init();
		process();
		if (verify() == false) {
			gTotalScore = PENALTY;
			break;
		}
	}
	printf("SCORE: %lld, TIME: %ld\n", gTotalScore, clock() - begin);
	return 0;
}
