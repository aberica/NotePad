#include <iostream>
#include <algorithm>
#include <bitset>
using namespace std;

struct Coord {
	int idx, x, y;
};

const int N_MAX = 100'010;
const int INF = 2'100'000'000;

int N;
Coord dot[N_MAX];
int x_sorted[N_MAX], y_sorted[N_MAX];
bitset<N_MAX> checked;

inline bool Flip(int idx, bool reverse, int left, int right, int top, int bottom) {
	bool flipped = false;
	for (int i = idx; i >= 0 && i < N; ) {
		if (checked[x_sorted[i]]) continue;
		if (dot[x_sorted[i]].x > left || dot[x_sorted[i]].x > right) break;
		if (dot[x_sorted[i]].y < top || dot[x_sorted[i]].y > bottom) continue;
		checked.flip(x_sorted[i]);
		flipped = true;

		if (reverse) i--;
		else i++;
	}
	return flipped;
}

bool Stick(int coin, int length, int dir) {
	printf("coin: %d, length: %d, dir: %d\n", coin, length, dir);
	if (coin == 0) return false;

	int left_idx = -1, right_idx = -1, top_idx = -1, bottom_idx = -1;
	for (int i = 0; i < N; i++) { if (!checked[x_sorted[i]]) { left_idx = i; break; }}
	for (int i = N - 1; i >= 0; i--) { if (!checked[x_sorted[i]]) { right_idx = i; break; }}
	for (int i = 0; i < N; i++) { if (!checked[y_sorted[i]]) { top_idx = i; break; }}
	for (int i = N - 1; i >= 0; i--) { if (!checked[y_sorted[i]]) { bottom_idx = i; break; }}

	int left = dot[x_sorted[left_idx]].x, right = dot[x_sorted[right_idx]].x, top = dot[y_sorted[top_idx]].y, bottom = dot[y_sorted[bottom_idx]].y;
	if (right - left <= length && bottom - top <= length) return true;

	if (dir == 0) {	// 좌상
		right = left + length;
		bottom = top + length;
		if (!Flip(left_idx, false, left, right, top, bottom)) return false;
	}
	else if (dir == 1) {	// 좌하
		right = left + length;
		top = bottom - length;
		if (!Flip(left_idx, false, left, right, top, bottom)) return false;
	}
	else if (dir == 2) {	// 우상
		left = right - length;
		bottom = top + length;
		if (!Flip(right_idx, true, left, right, top, bottom)) return false;
	}
	else if (dir == 3) {	// 우하
		left = right - length;
		top = bottom - length;
		if (!Flip(right_idx, true, left, right, top, bottom)) return false;
	}

	if (checked.all()) return true;
	else return (Stick(coin - 1, length, 0) || Stick(coin - 1, length, 1) || Stick(coin - 1, length, 2) || Stick(coin - 1, length, 3));
}

bool CheckAvail(int length) {
	checked.reset();

	Stick(3, length, 0);
	Stick(3, length, 1);
	Stick(3, length, 2);
	Stick(3, length, 3);

	return false;
}

int GetMinSize(int s, int e) {
	if (s == e) return s;

	int m = (s + e) / 2;
	// printf("%d -> %d\n", m, CheckAvail(m)==true);
	if (CheckAvail(m)) return GetMinSize(s, m);
	else return GetMinSize(m + 1, e);
}

int main() {
	int T; cin >> T;
	while(T--) {
		cin >> N;
		int x, y;
		for (int i = 0; i < N; i++) {
			cin >> x >> y;
			dot[i] = {i, x, y};
			x_sorted[i] = i;
			y_sorted[i] = i;
		}
		sort(x_sorted, x_sorted+N, [](int i, int j) {return dot[i].x < dot[j].x;});
		sort(y_sorted, y_sorted+N, [](int i, int j) {return dot[i].y < dot[j].y;});
		cout << GetMinSize(0, INF) << '\n';
	}
	return 0;
}
