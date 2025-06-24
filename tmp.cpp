#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

extern void init(int N, int K, int mId[], int sCity[], int eCity[], int mToll[]);
extern void add(int mId, int sCity, int eCity, int mToll);
extern void remove(int mId);
extern int calculate(int sCity, int eCity, int expected);

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
			ret = calculate(sCity, eCity, ans);
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
	freopen("sample_input.txt", "r", stdin);
	//freopen("ss.txt", "r", stdin);

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
#define INF 2100000000
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
struct Info {
	int city, tme;
	vector<int> city_trace, road_trace;

	Info(int _city = -1, int _tme = -1) : city(_city), tme(_tme) {
		city_trace.clear();
		city_trace.push_back(_city);
		road_trace.clear();
	}
	bool operator<(const Info& other) const {
		return tme > other.tme;
	}
};

vector<Node> cities;
map<int, Edge> roads;

void init(int N, int K, int mId[], int sCity[], int eCity[], int mTime[]) {
	cities.clear();
	roads.clear();
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

int calculate(int sCity, int eCity, int expected = -1) {
	int dist[CITY_MAX];
	priority_queue<Info> pq;

	fill(dist, dist + CITY_MAX, INF);
	dist[sCity] = 0;
	pq.push(Info(sCity, 0));
	Info first_info(-1, INF);
	while (!pq.empty()) {
		Info u = pq.top();
		pq.pop();
		if (u.tme > dist[u.city]) continue;

		if (u.city == eCity) {
			if (u.tme < first_info.tme) first_info = u;
			continue;
		}

		for (auto road : cities[u.city].out_roads) {
			if (road->destroied) continue;
			if (dist[road->to] <= u.tme + road->tme) continue;
			dist[road->to] = u.tme + road->tme;

			Info v(u);
			v.city = road->to, v.tme += road->tme, v.city_trace.push_back(road->to), v.road_trace.push_back(road->id);
			pq.push(v);
		}
	}
	if (first_info.city == -1) return -1;

	int city_order[CITY_MAX];
	fill(city_order, city_order + CITY_MAX, -1);
	for (auto i = 0; i < first_info.city_trace.size(); i++) {
		city_order[first_info.city_trace[i]] = i;
	}


	Info second_info(-1, -1);
	for (auto mId: first_info.road_trace) {
		roads[mId].destroied = true;

		fill(dist, dist + CITY_MAX, INF);
		dist[sCity] = 0;
		while (!pq.empty()) pq.pop();
		pq.push(Info(sCity, 0));
		bool road_not_exist = true;
		while (!pq.empty()) {
			Info u = pq.top();
			pq.pop();
			if (u.tme > dist[u.city]) continue;

			if (u.city == eCity) {
				if (u.tme > second_info.tme) second_info = u;
				road_not_exist = false;
				continue;
			}
			
			for (auto road : cities[u.city].out_roads) {
				if (road->destroied) continue;
				if (dist[road->to] <= u.tme + road->tme) continue;
				dist[road->to] = u.tme + road->tme;

				Info v(u);
				v.city = road->to, v.tme += road->tme, v.city_trace.push_back(road->to), v.road_trace.push_back(road->id);
				pq.push(v);
			}
		}
		roads[mId].destroied = false;

		if (road_not_exist) return -1;
	}
	if (second_info.city == -1) return -1;

	int output = second_info.tme - first_info.tme;
	//printf("  Expected: %d, Output: %d / %d - %d\n", expected, output, second_info.tme, first_info.tme);
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

오답노트
1. 다익스트라 vistied 사용 -> dist를 사용해야. 그리고 값 업데이트와 조건문continue 위치 정정
2. second_info는 최대값을 구함에 주의. 가는 길이 없을 경우 -1을 return 해야
3. 길을 임의로 하나씩 삭제하고 복구할 때, 길을 복구하고 나서 road_not_exist일때 return -1을 해줘야
4. 추가발전방안 : node_trace를 저장하여, 길을 없앤 부분부터 탐색을 시작하여 second_info를 구할 때, 아는 길로 접어들었을 경우 더이상 계산 X하는 방법. 근데 이거 노드방문한지 안한지, 세심한 주의가 필요해 보임.
*/
