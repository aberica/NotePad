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

bool Stick(int depth, int dir) {
	if (depth == 3) {
		if (checked.all()) return true;
		else return false;
	}

	int left = -1, right = -1, top = -1, bottom = -1;
	for (int i = 0; i < N; i++) { if (!checked[x_sorted[i]]) { left = i; break; }}
	for (int i = N - 1; i >= 0; i--) { if (!checked[x_sorted[i]]) { right = i; break; }}
	for (int i = 0; i < N; i++) { if (!checked[y_sorted[i]]) { top = i; break; }}
	for (int i = N - 1; i >= 0; i--) { if (!checked[y_sorted[i]]) { bottom = i; break; }}

	if (dir == 0) {	// 좌상
		
	}
}

bool CheckAvail(int length) {
	checked.reset();

	Stick(0, 0);
	Stick(0, 1);
	Stick(0, 2);
	Stick(0, 3);

	for (int i = 0; i < 3; i++) {
		int left = -1, right = -1, top = -1, bottom = -1;
		for (int i = 0; i < N; i++) { if (!checked[x_sorted[i]]) { left = i; break; }}
		for (int i = N - 1; i >= 0; i--) { if (!checked[x_sorted[i]]) { right = i; break; }}
		for (int i = 0; i < N; i++) { if (!checked[y_sorted[i]]) { top = i; break; }}
		for (int i = N - 1; i >= 0; i--) { if (!checked[y_sorted[i]]) { bottom = i; break; }}



		if (checked.all()) return true;
	}
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
