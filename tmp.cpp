#include <iostream>
#include <algorithm>
#include <bitset>
#include <vector>
#include <random>
#define ll long long
using namespace std;

struct Coord {
	ll idx, x, y;
};

const ll N_MAX = 100'010;
const ll INF = 2'100'000'000;


ll N;
Coord dot[N_MAX];
ll x_sorted[N_MAX], y_sorted[N_MAX];
bitset<N_MAX> covered;

clock_t s_time, e_time;
double duration;

vector<ll> Cover(ll idx, bool reverse, ll left, ll right, ll top, ll bottom) {
	vector<ll> covered_idx;
	for (ll i = idx; i >= 0 && i < N; i = reverse ? i - 1 : i + 1) {
		if (covered[x_sorted[i]]) continue;
		if (dot[x_sorted[i]].x < left || dot[x_sorted[i]].x > right) break;
		if (dot[x_sorted[i]].y < top || dot[x_sorted[i]].y > bottom) continue;
		covered.set(x_sorted[i]);
		covered_idx.push_back(x_sorted[i]);
	}
	return covered_idx;
}

bool isAllSet(ll l, ll r) {
	for (ll i = l; i <= r; ++i) {
		if (!covered[i]) return false;
	}
	return true;
}
bool Stick(ll coin, ll length, ll dir, ll l_st = 0, ll r_st = N - 1, ll t_st = 0, ll b_st = N - 1) {
	if (coin == 0) return false;

	ll left_idx = -1, right_idx = -1, top_idx = -1, bottom_idx = -1;
	for (ll i = l_st; i < N; i++) { if (!covered[x_sorted[i]]) { left_idx = i; break; } }
	for (ll i = r_st; i >= 0; i--) { if (!covered[x_sorted[i]]) { right_idx = i; break; } }
	for (ll i = t_st; i < N; i++) { if (!covered[y_sorted[i]]) { top_idx = i; break; } }
	for (ll i = b_st; i >= 0; i--) { if (!covered[y_sorted[i]]) { bottom_idx = i; break; } }
	if (left_idx == -1 || top_idx == -1 || right_idx == -1 || bottom_idx == -1) return true;
	ll left = dot[x_sorted[left_idx]].x, right = dot[x_sorted[right_idx]].x, top = dot[y_sorted[top_idx]].y, bottom = dot[y_sorted[bottom_idx]].y;
	if (right - left <= length && bottom - top <= length) return true;

	vector<ll> covered_idx;
	if (dir == 0) {	// 좌상
		right = left + length;
		bottom = top + length;
		covered_idx = Cover(left_idx, false, left, right, top, bottom);
	}
	else if (dir == 1) {	// 좌하
		right = left + length;
		top = bottom - length;
		covered_idx = Cover(left_idx, false, left, right, top, bottom);
	}
	else if (dir == 2) {	// 우상
		left = right - length;
		bottom = top + length;
		covered_idx = Cover(right_idx, true, left, right, top, bottom);
	}
	else if (dir == 3) {	// 우하
		left = right - length;
		top = bottom - length;
		covered_idx = Cover(right_idx, true, left, right, top, bottom);
	}
	if (covered_idx.size() == 0) return false;
	else if (isAllSet(0, N - 1)) return true;


	bool rt = Stick(coin - 1, length, 0, left_idx, right_idx, top_idx, bottom_idx) || \
		Stick(coin - 1, length, 1, left_idx, right_idx, top_idx, bottom_idx) || \
		Stick(coin - 1, length, 2, left_idx, right_idx, top_idx, bottom_idx) || \
		Stick(coin - 1, length, 3, left_idx, right_idx, top_idx, bottom_idx);
	for (auto i : covered_idx) covered.reset(i);
	return rt;
}

bool CheckAvail(ll length) {
	covered.reset();
	if (Stick(3, length, 0)) {
		//e_time = clock();
		//duration = static_cast<double>(e_time - s_time) / CLOCKS_PER_SEC;
		//printf("Stick(length: %lld, dir: %lld): %f - true\n", length, 0, duration);
		return true;
	}
	//e_time = clock();
	//duration = static_cast<double>(e_time - s_time) / CLOCKS_PER_SEC;
	//printf("Stick(length: %lld, dir: %lld): %f - false\n", length, 0, duration);
	if (Stick(3, length, 1)) return true;
	if (Stick(3, length, 2)) return true;
	if (Stick(3, length, 3)) return true;

	return false;
}

ll GetMinSize(ll s, ll e) {
	if (s == e) return s;

	ll m = (s + e) / 2;
	if (CheckAvail(m)) return GetMinSize(s, m);
	else return GetMinSize(m + 1, e);
}

void TimeTest() {
	std::srand(static_cast<unsigned ll>(std::time(nullptr)));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<long long> gen_dist(-1000000000, 1000000000);

	ll T = rand() % 10;
	while (T--) {
		s_time = clock();

		//N = rand() % 100'000 + 1;
		N = 10'000;
		ll x, y, max_x = -INF, min_x = INF, max_y = -INF, min_y = INF;
		for (ll i = 0; i < N; i++) {
			x = gen_dist(gen);
			y = gen_dist(gen);
			dot[i] = { i, x, y };
			x_sorted[i] = i;
			y_sorted[i] = i;

			max_x = max(max_x, x);
			min_x = min(min_x, x);
			max_y = max(max_y, y);
			min_y = min(min_y, y);
		}
		sort(x_sorted, x_sorted + N, [](ll i, ll j) {return dot[i].x < dot[j].x; });
		sort(y_sorted, y_sorted + N, [](ll i, ll j) {return dot[i].y < dot[j].y; });
		//e_time = clock();
		//duration = static_cast<double>(e_time - s_time) / CLOCKS_PER_SEC;
		//printf("sort시간: %f\n", duration);
		//printf("max_x: %lld, min_x: %lld, max_y: %lld, min_y: %lld\n", max_x, min_x, max_y, min_y);
		//printf("max(max_x - min_x, max_y - min_y): %lld\n", max((long long)0 + max_x - min_x, (long long)0 + max_y - min_y));
		cout << GetMinSize(0, max(max_x - min_x, max_y - min_y)) << '\n';

		e_time = clock();
		duration = static_cast<double>(e_time - s_time) / CLOCKS_PER_SEC;
		printf("실행시간: %f\n\n", duration);
	}
}

int main() {
	//TimeTest(); return 0;

	ll T; cin >> T;
	while (T--) {
		cin >> N;
		ll x, y, max_x = -INF, min_x = INF, max_y = -INF, min_y = INF;
		for (ll i = 0; i < N; i++) {
			cin >> x >> y;
			dot[i] = { i, x, y };
			x_sorted[i] = i;
			y_sorted[i] = i;
			max_x = max(max_x, x);
			min_x = min(min_x, x);
			max_y = max(max_y, y);
			min_y = min(min_y, y);
		}
		sort(x_sorted, x_sorted + N, [](ll i, ll j) {return dot[i].x < dot[j].x; });
		sort(y_sorted, y_sorted + N, [](ll i, ll j) {return dot[i].y < dot[j].y; });
		cout << GetMinSize(0, max(max_x - min_x, max_y - min_y)) << '\n';
	}
	return 0;
}
