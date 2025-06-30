#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_map>
#define STORE_MAX 100'001
#define INF 2'100'000'000
using namespace std;

struct Dists{
	int d[3], idx;
	Dists(int _a=INF, int _b=INF, int _c=INF, int _idx = -1) {
		d[0] = _a;
		d[1] = _b;
		d[2] = _c;
		idx = _idx;
	}
};

int city_num, apartments[3];
vector<pair<int, int>> roads[STORE_MAX];
Dists dists[STORE_MAX];
bool affordable[STORE_MAX];
int seg_tree[STORE_MAX * 4];
int sorted_idx[STORE_MAX];

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
	fill(dists, dists + STORE_MAX, Dists(INF, INF, INF));
	for (int i = 1; i <= city_num; i++) dists[i].idx = i;
	for (int i = 0; i < 3; i++) {
		dists[apartments[i]].d[i] = 0;
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
		pq.push({0, apartments[i]});
		while(!pq.empty()) {
			auto u = pq.top();
			pq.pop();
			if (u.first > dists[u.second].d[i]) continue;
			for (auto road : roads[u.second]) {
				pair<int, int> v = {road.first + u.first, road.second};
				if (v.first >= dists[v.second].d[i]) continue;
				dists[v.second].d[i] = v.first;
				pq.push(v);
			}
		}
	}
}
void SortDist() {
	auto cmp = [](const Dists a, const Dists b) {
		if (a.d[0] != b.d[0]) return a.d[0] < b.d[0];
		else return a.d[1] > b.d[1];
	};
	sort(dists + 1, dists + 1 + city_num, cmp);
	for (int i = 1; i <= city_num; i++) sorted_idx[dists[i].idx] = i;
	// for (int i = 1; i <= city_num; i++) city_idxs[i] = i;		// [1st] -> city_idx
	// sort(city_idxs + 1, city_idxs + 1 + city_num, cmp);
	// for (int i = 1; i <= city_num; i++) city_orders[city_idxs[i]] = i;	// [city_idx] -> 1st
}
int query(int node, int start, int end, int left, int right) {
	if (right < start || end < left) return INF;
	if (left <= start && end <= right) {
		return seg_tree[node];
	}
	int mid = (start + end) / 2;
	return min(query(node * 2, start, mid, left, right), query(node * 2 + 1, mid + 1, end, left, right));
}
void update(int node, int start, int end, int val, int pos) {
	if (pos < start || end < pos) return;
	if (start == end) {
		seg_tree[node] = min(seg_tree[node], val);
		return;
	}
	int mid = (start + end) / 2;
	update(node * 2, start, mid, val, pos);
	update(node * 2, mid + 1, end, val, pos);
	seg_tree[node] = min(seg_tree[node * 2], seg_tree[node * 2 + 1]);
}
void CalAfford() {
	vector<int> b_values;
	for (int i = 1; i <= city_num; i++) b_values.push_back(dists[i].d[1]);
	sort(b_values.begin(), b_values.end());
	b_values.erase(unique(b_values.begin(), b_values.end()), b_values.end());

	unordered_map<int, int> b_comp;
	for (int i = 0; i < (int)b_values.size(); i++) b_comp[b_values[i]] = i + 1;

	fill(seg_tree, seg_tree + STORE_MAX * 4, INF);
	fill(affordable, affordable + STORE_MAX, false);
	for (int i = 1; i <= city_num; i++) {
		int cur_b = b_comp[dists[i].d[1]];
		int min_val = query(1, 1, city_num, 1, cur_b - 1);  // B' < B
		if (min_val < dists[i].d[2]) affordable[i] = false;
		else affordable[i] = true;
		update(1, 1, city_num, dists[i].d[2] + 1, cur_b);  // z < c 조건 유도
	}
}

int main() {
	Init();
	CalDist();
	SortDist();
	CalAfford();

	int query_num; cin >> query_num;
	for (auto q = 0; q < query_num; q++) {
		int idx; cin >> idx;
		
		if (affordable[sorted_idx[idx]]) printf("YES\n");
		else printf("NO\n");
	}
	return 0;
}
