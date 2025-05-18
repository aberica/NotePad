#include <stdio.h>
#define ABS(x) ((x) < 0 ? -(x) : (x))

enum {
    EMPTY = 0, HARUBANG = 1, HOUSE = 2
};

const int TC = 10;
const int MAX_HARUBANG = 2000;
const int MAP_SIZE = 1024;

////////////////////////////////////////////////////////////////

struct Point {
    unsigned int x, y;
};
static int island[MAP_SIZE][MAP_SIZE];
static int harubang_cnt;
static Point harubang[MAX_HARUBANG];
static int used_harubang_cnt;
static int house_cnt;
static int house_area[MAX_HARUBANG / 4];
static int total_score;

////////////////////////////////////////////////////////////////

static unsigned int random(void) {
	static unsigned long long seed = 5;
	return ((unsigned int)((seed = seed * 24214903917ULL + 11ULL) >> 16));
}
int GetHarubangInfo(Point p[]) {
    for (int i = 0; i < harubang_cnt; i++) {
        p[i] = harubang[i];
    }
    return harubang_cnt;
}
void House(Point p[]) {
    for (int i = 0; i < 4; ++i) {
        if (p[i].x < 0 || p[i].x >= MAP_SIZE || p[i].y < 0 || p[i].y >= MAP_SIZE)
            return;
    }

    int minX = MAP_SIZE * 2, maxX = -1, minY = MAP_SIZE * 2, maxY = -1;
    for (int i = 0; i < 4; i++) {
        if (p[i].x < minX) minX = p[i].x;
        if (p[i].x > maxX) maxX = p[i].x;
        if (p[i].y < minY) minY = p[i].y;
        if (p[i].y > maxY) maxY = p[i].y;
    }

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            if (island[y][x] == HARUBANG) {
                used_harubang_cnt++;
                island[y][x] = EMPTY;
            }
            else if (island[y][x] == HOUSE) {
                harubang_cnt = -1;
                return;
            }
        }
    }

    for (int y = minY; y <= maxY; y++) 
        for (int x = minX; x <= maxX; x++) 
            island[y][x] = HOUSE;
    
    house_area[house_cnt] = (maxX - minX + 1) * (maxY - minY + 1);
    house_cnt++;
}
void Init() {
    for (int y = 0; y < MAP_SIZE; y++) 
        for (int x = 0; x < MAP_SIZE; x++) 
            island[y][x] = EMPTY;

    harubang_cnt = MAX_HARUBANG / 2 + random() % (MAX_HARUBANG);
    for (int i = 0; i < harubang_cnt; i++) {
        harubang[i] = {random() % MAP_SIZE, random() % MAP_SIZE};
        island[harubang[i].y][harubang[i].x] = HARUBANG;
    }
    used_harubang_cnt = 0;

    house_cnt = 0;
}
extern void Process();
bool Verify() {
    if (used_harubang_cnt != harubang_cnt) return false;
    
    float area_sum = 0;
    for (int i = 0; i < house_cnt; i++) {
        total_score += i * i;
        area_sum += house_area[i];
    }
    float area_avg = area_sum / (float)house_cnt;
    for (int i = 0; i < house_cnt; i++) {
        total_score -= ABS(area_avg - house_area[i]) / 10;
    }

    return true;
}

int main() {
    int total_score = 0;
    for (int i = 0; i < TC; i++) {
        Init();
        Process();
        bool result = Verify();
        if (!result) total_score -= 1'000'000'000;
    }
    printf("SCORE : %d\n", total_score);
}
