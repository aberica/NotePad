#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#define MAX 10'000'000
#define ll long long
using namespace std;

struct Coord {
	int x, y;
	Coord(int _x=MAX*(-2), int _y=MAX*(-2)) :x(_x), y(_y) {	}
	bool operator<(const Coord& p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
	Coord operator+(const Coord& p) const {
		return Coord(x + p.x, y + p.y);
	}
	Coord operator/(const int& n) const {
		return Coord(x / n, y / n);
	}
};

vector<Coord> points;
Coord anchor_point;

void Init() {
	points.clear();
	int point_num; cin >> point_num;
	while(point_num--) {
		int x, y; cin >> x >> y;
		points.push_back(Coord(x, y));
	}
}
int ccw(const Coord& a, const Coord& b, const Coord& c) {
    int cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return cross;
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
vector<Coord*> GetConvexHull() {
	anchor_point = *min_element(points.begin(), points.end());
	sort(points.begin(), points.end(), ccw_cmp);		

	vector<Coord*> hull;
	for (int i = 0; i < points.size(); i++) {
		while (hull.size() >= 2 && ccw(*hull[hull.size() - 2], *hull[hull.size() - 1], points[i]) <= 0) {
			hull.pop_back();
		}
		hull.push_back(&points[i]);
	}
	return hull;
}
int GetDist(const Coord& p1, const Coord& p2) {
	ll dx = p1.x - p2.x, dy = p1.y - p2.y;
	return sqrt(dx*dx+dy*dy);
}
int main() {
	int T; cin >> T;
	while(T--) {
		Init();
		vector<Coord*> hull = GetConvexHull();
		
		Coord center(0, 0);
		for (auto& p : hull) center = center + *p;
		center = center / hull.size();

		priority_queue<pair<int, Coord*>> dist;
		for (auto& p : hull) dist.push({GetDist(center, *p), p});

		

	}
	return 0;
}
