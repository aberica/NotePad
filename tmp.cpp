#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_set>
#define STORE_MAX 100'001
#define INF 2'100'000'000
using namespace std;

int city_num, apartments[3];
vector<pair<int, int>> roads[STORE_MAX];
int dists[3][STORE_MAX];
int city_idxs[STORE_MAX], city_orders[STORE_MAX];

void Init() {
	cin >> city_num;
	cin >> apartments[0] >> apartments[1] >> apartments[2];
	int road_num; cin >> road_num;
	for (int i = 0; i < road_num; i++) {
		int from, to, dist; cin >> from >> to >> dist;
		roads[from].push_back({dist, to});
		roads[to].push_back({dist, from});
	}
}
void CalDist() {
	for (int i = 0; i < 3; i++) {
		fill(dists[i], dists[i] + STORE_MAX, INF);
		dists[i][apartments[i]] = 0;
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
		pq.push({0, apartments[i]});
		while(!pq.empty()) {
			auto u = pq.top();
			pq.pop();
			if (u.first > dists[i][u.second]) continue;
			for (auto road : roads[u.second]) {
				pair<int, int> v = {road.first + u.first, road.second};
				if (v.first >= dists[i][v.second]) continue;
				dists[i][v.second] = v.first;
				pq.push(v);
			}
		}
	}
}

void SortDist() {
	auto cmp = [](const int& a, const int& b) {
		if (dists[0][a] != dists[0][b]) return dists[0][a] < dists[0][b];
		else if (dists[1][a] != dists[1][b]) return dists[1][a] < dists[1][b];
		else return dists[2][a] < dists[2][b];
	};
	for (int i = 1; i <= city_num; i++) city_idxs[i] = i;		// [1st] -> city_idx
	sort(city_idxs + 1, city_idxs + 1 + city_num, cmp);
	for (int i = 1; i <= city_num; i++) city_orders[city_idxs[i]] = i;	// [city_idx] -> 1st
}


int main() {
	Init();
	CalDist();
	SortDist();

	int query_num; cin >> query_num;
	for (auto q = 0; q < query_num; q++) {
		int idx; cin >> idx;

		bool dominated = false;
		for (int i = 1; i < city_orders[idx]; i++) {
			int prior_idx = city_idxs[i];
			if (dists[0][prior_idx] < dists[0][idx] &&
				dists[1][prior_idx] < dists[1][idx] &&
				dists[2][prior_idx] < dists[2][idx]) {
				dominated = true;
				break;
			}
		}
		if (dominated) printf("NO\n");
		else printf("YES\n");
	}
	return 0;
}


'''
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

const int INF = 2'000'000'000;

struct Child {
    int a, b, c;
    bool operator<(const Child& other) const {
        if (a != other.a) return a < other.a;
        return b > other.b; // a가 같을 경우 b 큰 게 먼저!
    }
};

vector<int> segTree;

void update(int node, int start, int end, int index, int value) {
    if (index < start || index > end) return;
    if (start == end) {
        segTree[node] = min(segTree[node], value);
        return;
    }
    int mid = (start + end) / 2;
    update(node * 2, start, mid, index, value);
    update(node * 2 + 1, mid + 1, end, index, value);
    segTree[node] = min(segTree[node * 2], segTree[node * 2 + 1]);
}

int query(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return INF;
    if (l <= start && end <= r) return segTree[node];
    int mid = (start + end) / 2;
    return min(
        query(node * 2, start, mid, l, r),
        query(node * 2 + 1, mid + 1, end, l, r)
    );
}

int main() {
    int n;
    cin >> n;
    vector<Child> kids(n);
    vector<int> b_values;

    for (int i = 0; i < n; ++i) {
        cin >> kids[i].a >> kids[i].b >> kids[i].c;
        b_values.push_back(kids[i].b);
    }

    // 1. 좌표 압축
    sort(b_values.begin(), b_values.end());
    b_values.erase(unique(b_values.begin(), b_values.end()), b_values.end());
    map<int, int> b_index;
    for (int i = 0; i < b_values.size(); ++i)
        b_index[b_values[i]] = i;

    int M = b_values.size();
    segTree.assign(4 * M, INF); // 초기화

    // 2. a 기준 정렬, a 같으면 b 큰 것부터
    sort(kids.begin(), kids.end());

    int installable = 0;
    for (auto& kid : kids) {
        int b_idx = b_index[kid.b];
        // 3. b < 현재 b인 애들 중 최소 c 찾기
        int min_c = query(1, 0, M - 1, 0, b_idx - 1);
        if (min_c > kid.c) {
            installable++; // 자신보다 완전 작은 애가 없음
        }
        // 4. 현재 아이 정보를 세그 트리에 업데이트
        update(1, 0, M - 1, b_idx, kid.c);
    }

    cout << installable << '\n';
    return 0;
}
'''
