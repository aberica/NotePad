#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#define MAX 10'000'000
#define ll long long
using namespace std;

struct Coord {
	int x, y;
	Coord(int _x = MAX * (-2), int _y = MAX * (-2)) :x(_x), y(_y) {}
	bool operator<(const Coord& p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
	Coord operator+(const Coord& p) const {
		return Coord(x + p.x, y + p.y);
	}
	Coord operator-(const Coord& p) const {
		return Coord(x - p.x, y - p.y);
	}
};

vector<Coord> points, hull;
Coord anchor_point;

void Init() {
	points.clear();
	int point_num; cin >> point_num;
	while (point_num--) {
		int x, y; cin >> x >> y;
		points.push_back(Coord(x, y));
	}
}
int ccw(const Coord& a, const Coord& b, const Coord& c) {
	ll cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	if (cross > 0) return 1;
	else if (cross == 0) return 0;
	else return -1;
}
bool ccw_cmp(const Coord& p1, const Coord& p2) {
	int res = ccw(anchor_point, p1, p2);
	if (res == 0) {
		int dx1 = p1.x - anchor_point.x, dy1 = p1.y - anchor_point.y;
		int dx2 = p2.x - anchor_point.x, dy2 = p2.y - anchor_point.y;
		return dx1 * dx1 + dy1 * dy1 < dx2 * dx2 + dy2 * dy2;
	}
	return res > 0;
}
void MakeConvexHull() {
	anchor_point = *min_element(points.begin(), points.end());
	sort(points.begin(), points.end(), ccw_cmp);

	hull.clear();
	for (int i = 0; i < points.size(); i++) {
		while (hull.size() >= 2 && ccw(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0) {
			hull.pop_back();
		}
		hull.push_back(points[i]);
	}
}
ll GetDist(const Coord& p1, const Coord& p2) {
	Coord d = p1 - p2;
	return 1LL * d.x * d.x + 1LL * d.y * d.y;
}
void Process() {
	int a = 0, b = 1;
	ll mx_dist = -1;
	pair<int, int> output(-1, -1);

	Coord base(0, 0);
	while (a < hull.size()) {
		ll dist = GetDist(hull[a], hull[b]);
		if (dist > mx_dist) {
			mx_dist = dist;
			output = { a, b };
		}

		Coord da = hull[(a + 1) % hull.size()] - hull[a], db = hull[(b + 1) % hull.size()] - hull[b];
		if (ccw(base, da, db) > 0) b = (b + 1) % hull.size();
		else a++;
	}
	printf("%d %d %d %d\n", hull[output.first].x, hull[output.first].y, hull[output.second].x, hull[output.second].y);
}

int main() {
	int T; cin >> T;
	while (T--) {
		Init();
		MakeConvexHull();
		Process();
	}
	return 0;
}
