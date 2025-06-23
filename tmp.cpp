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
#include <vector>
#include <map>
#include <queue>
#define CITY_MAX 1'000
using namespace std;

struct Edge {
	int id, from, to, tme;
	bool destroied;
	Edge(int _id = -1, int _from = -1, int _to = -1, int _tme = -1) : id(_id), from(_from), to(_to), tme(_tme) {
		destroied = false;
	}
};
struct Node {
	int id;
	vector<Edge*> in_roads, out_roads;

	Node(int _id = -1) : id(_id) {
		in_roads.clear();
		out_roads.clear();
	}
};

vector<Node> cities;
map<int, Edge> roads;

void init(int N, int K, int mId[], int sCity[], int eCity[], int mTime[]) {
	for (int i = 0; i < N; i++) cities.push_back(Node(i));
	for (int i = 0; i < K; i++) add(mId[i], sCity[i], eCity[i], mTime[i]);
	return;
}

int getSize(vector<Edge*> vs) {
	int cnt = 0;
	for (auto& v : vs) {
		if (v->destroied == false) cnt++;
	}
	return cnt;
}
void add(int mId, int sCity, int eCity, int mTime) {
	roads[mId] = Edge(mId, sCity, eCity, mTime);
	cities[sCity].out_roads.push_back(&roads[mId]);
	cities[eCity].in_roads.push_back(&roads[mId]);
	return;
}

void remove(int mId) {
	roads[mId].destroied = true;
	return;
}

int calculate(int sCity, int eCity) {
	pair<int, int> visited[CITY_MAX];		// {first arrived time, minimum time diff}
	for (int i = 0; i < cities.size(); i++) visited[i] = { -1, -1 };
	priority_queue<pair<int, int>> pq;	// {-time, city idx}
	pq.push({ 0, sCity });
	visited[sCity] = { 0, 0 };

	int output = 0;
	while (!pq.empty()) {
		int u_tme = -pq.top().first, u_city = pq.top().second;
		pq.pop();

		if (u_city == eCity) {
			continue;
		}

		for (auto road : cities[u_city].out_roads) {
			if (road->destroied) continue;

			int v_tme = -(u_tme + road->tme), v_city = road->to;
			if (visited[v_city].first != -1) {
				visited[v_city].second = min(visited[v_city].second, v_tme - visited[v_city].first);
				output = max(output, visited[v_city].second);
			}
			else {
				visited[v_city].first = v_tme;
				pq.push({ v_tme, v_city });
			}
		}
	}

	return output;
}


/*
Parameters
N: 도시의 개수 ( 5 ≤ N ≤ 1,000 )
K: 도로의 개수 ( 7 ≤ K ≤ 5,000 )
mId: 도로의 ID ( 1 ≤ mId ≤ 1,000,000,000 )
sCity: 도로의 출발 도시 ( 0 ≤ sCity < N )
eCity: 도로의 도착 도시 ( 0 ≤ eCity < N )
mTime: 도로의 소요 시간 ( 1 ≤ mTime ≤ 1,000 )

[제약사항]
1. 각 테스트 케이스 시작 시 init() 함수가 호출된다.
2. 각 테스트 케이스에서 add() 함수의 호출 횟수는 1,500 이하이다.
3. 각 테스트 케이스에서 remove() 함수의 호출 횟수는 500 이하이다.
4. 각 테스트 케이스에서 calculate() 함수의 호출 횟수는 200 이하이다.


*/
