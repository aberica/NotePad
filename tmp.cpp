#include <iostream>
#include <cstring>
using namespace std;

struct Coord {
	int x, y;
};

const int N_MAX = 100'010;
const int INF = 2'100'000'000;

int N;
Coord dot1[N_MAX], dot2[N_MAX];
bool avail[N_MAX];

bool Stick1(Coord dot[], int sze) {
	int from = -1, to = -1;
	for (int i = 0; i < N; i++) {
		if (avail[i]) continue;
		if (from == -1 || dot[i].y < dot[from].y || dot[i].y == dot[from].y && dot[i].x < dot[from].x) {
			from = i;
		}
	}
	for (int i = 0; i < N; i++) {
		if (avail[i]) continue;
		if (dot[i].y <= dot[from].y + sze && dot[i].x >= dot[from].x - sze && dot[i].x <= dot[from].x) {
			if (to == -1 || dot[i].x < dot[to].x ) {
				to = i;
			}
		}
	}
	if (from == -1) return true;
	for (int i = 0; i < N; i++) {
		if (dot[i].y >= dot[from].y && dot[i].y <= dot[from].y + sze && dot[i].x >= min(dot[to].x, dot[from].x) && dot[i].x <= min(dot[to].x, dot[from].x) + sze) {
			avail[i] = true;
		}
	}
	// printf("Stick1- {%d, %d} : {%d, %d}\n", min(dot[to].x, dot[from].x), dot[from].y, min(dot[to].x, dot[from].x) + sze, dot[from].y + sze);
	return false;
}
bool Stick2(Coord dot[], int sze) {
	int from = -1, to = -1;
	for (int i = 0; i < N; i++) {
		if (avail[i]) continue;
		if (from == -1 || dot[i].y < dot[from].y || dot[i].y == dot[from].y && dot[i].x > dot[from].x) {
			from = i;
		}
	}
	for (int i = 0; i < N; i++) {
		if (avail[i]) continue;
		if (dot[i].y <= dot[from].y + sze && dot[i].x <= dot[from].x + sze && dot[i].x >= dot[from].x) {
			if (to == -1 || dot[i].x > dot[to].x ) {
				to = i;
			}
		}
	}
	if (from == -1) return true;
	for (int i = 0; i < N; i++) {
		if (dot[i].y >= dot[from].y && dot[i].y <= dot[from].y + sze && dot[i].x >= max(dot[to].x, dot[from].x) - sze && dot[i].x <= max(dot[to].x, dot[from].x)) {
			avail[i] = true;
		}
	}
	// printf("Stick2- {%d, %d} : {%d, %d}\n", min(dot[to].x, dot[from].x), dot[from].y, min(dot[to].x, dot[from].x) + sze, dot[from].y + sze);
	return false;
}

bool CheckAvail(int sze) {
	// 좌상 -> 우상
	memset(avail, false, sizeof avail);
	if (Stick1(dot1, sze)) return true;
	if (Stick1(dot1, sze)) return true;
	if (Stick1(dot1, sze)) return true;
	bool all_avail = true;
	for (int i = 0; i < N; i++) if (avail[i] == false) { all_avail = false; break; }
	if (all_avail) return true;

	// 좌하 -> 우하
	memset(avail, false, sizeof avail);
	if (Stick2(dot1, sze)) return true;
	if (Stick2(dot1, sze)) return true;
	if (Stick2(dot1, sze)) return true;
	all_avail = true;
	for (int i = 0; i < N; i++) if (avail[i] == false) { all_avail = false; break; }
	if (all_avail) return true;

	// 상좌 -> 하좌
	memset(avail, false, sizeof avail);
	if (Stick1(dot2, sze)) return true;
	if (Stick1(dot2, sze)) return true;
	if (Stick1(dot2, sze)) return true;
	all_avail = true;
	for (int i = 0; i < N; i++) if (avail[i] == false) { all_avail = false; break; }
	if (all_avail) return true;

	// 상우 -> 하우
	memset(avail, false, sizeof avail);
	if (Stick2(dot2, sze)) return true;
	if (Stick2(dot2, sze)) return true;
	if (Stick2(dot2, sze)) return true;
	all_avail = true;
	for (int i = 0; i < N; i++) if (avail[i] == false) { all_avail = false; break; }
	if (all_avail) return true;
	else return false;
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
		for (int i = 0; i < N; i++) {
			cin >> dot1[i].x >> dot1[i].y;
			dot2[i].x = dot1[i].y, dot2[i].y = dot1[i].x;
		}
		cout << GetMinSize(0, INF) << '\n';
	}
	return 0;
}

/*
이진탐색(INF) -> log(2e9) = 31
3초 : 3e9
1e8이 남음
N: 1e5
log(N) = log(128*1e3) = 17
log(2e9) * Nlog(N) = 50*1e5 = 5*1e6 (OK)

10454
*/
