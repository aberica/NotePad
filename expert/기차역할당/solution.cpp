#include <stdio.h>
#define HOUSE_NUM 100'000
#define STATION_NUM 5
#define MAX_ALLOC 22'000
typedef struct {
	int idx;
	short from, to;
	double dist_diff;
} ChangeInfo;

double shouse[HOUSE_NUM];
int sindex[HOUSE_NUM];
short salloc[HOUSE_NUM];

int alloc_num[STATION_NUM];
int changable_area[STATION_NUM - 1][2];
ChangeInfo changable_candi[(MAX_ALLOC - HOUSE_NUM / STATION_NUM) * STATION_NUM * (STATION_NUM - 1)];		// 40'000

namespace debug {
	bool debug = false;
	void PrintStatus(double station[]) {
		if (!debug) return;
		printf("station   - %lf %lf %lf %lf %lf\n", station[0], station[1], station[2], station[3], station[4]);
		printf("alloc_num - %d %d %d %d %d\n\n", alloc_num[0], alloc_num[1], alloc_num[2], alloc_num[3], alloc_num[4]);
	}
};

int MedianOfThree(double house[], int low, int mid, int high) {
	double a = house[low], b = house[mid], c = house[high];

	if (a <= b && b <= c || c <= b && b <= a) return mid;
	else if (b <= a && a <= c || c <= a && a <= b) return low;
	else return high;
}
void Swap(double& a, double& b) {
	double tmp = a;
	a = b;
	b = tmp;
}
void Swap(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}
int Partition(double house[], int index[], int low, int high) {
	int mid = (low + high) / 2;
	int pivot_index = MedianOfThree(house, low, mid, high);
	Swap(house[pivot_index], house[high]);
	Swap(index[pivot_index], index[high]);
	pivot_index = high;
	double pivot = house[pivot_index];

	high--;
	while (1) {
		while (house[low] < pivot) low++;
		while (house[high] > pivot) high--;

		if (low >= high) break;
		else {
			Swap(house[low], house[high]);
			Swap(index[low], index[high]);
			low++;
		}
	}
	Swap(house[low], house[pivot_index]);
	Swap(index[low], index[pivot_index]);
	return low;
}
void QuickSort(double house[], int index[], int low, int high) {
	if (low < high) {
		int pt = Partition(house, index, low, high);

		QuickSort(house, index, low, pt - 1);
		QuickSort(house, index, pt + 1, high);
	}
}
void Init(short alloc[]) {
	for (int i = 0, num_sum = 0; i < STATION_NUM; i++) {
		alloc_num[i] = HOUSE_NUM / STATION_NUM;
		for (int j = num_sum; j < num_sum + alloc_num[i]; j++) {
			alloc[j] = i;
		}
		num_sum += alloc_num[i];
	}
	for (int i = 1; i <= STATION_NUM - 1; i++) {
		int avg_num = HOUSE_NUM / STATION_NUM;
		changable_area[i - 1][0] = avg_num * i - (MAX_ALLOC - avg_num) * (STATION_NUM - i);
		changable_area[i - 1][1] = avg_num * i + (MAX_ALLOC - avg_num) * i;
	}
	
}
void AllocStation2Avg(const double house[], short alloc[], double station[]) {
	double pos_sum[STATION_NUM] = { 0, };
	for (int i = 0; i < HOUSE_NUM; i++) pos_sum[alloc[i]] += house[i];

	for (int i = 0; i < STATION_NUM; i++) station[i] = pos_sum[i] / alloc_num[i];
}
short GetCloserStation(double pos, int idx, double station[], double& dist_diff) {
	double dist_1 = station[idx] - pos;
	if (dist_1 < 0) dist_1 *= -1;
	double dist_2 = station[idx + 1] - pos;
	if (dist_2 < 0) dist_2 *= -1;
	dist_diff = dist_1 - dist_2;
	if (dist_diff < 0) dist_diff *= -1;

	return dist_1 < dist_2 ? idx : idx + 1;
}
int FindChangableCandi(const double house[], double station[], short alloc[], ChangeInfo change_candi[]) {
	int idx = 0;
	for (int i = 0; i < STATION_NUM - 1; i++) {
		for (int j = (changable_area[i][0] + changable_area[i][1]) / 2; j >= changable_area[i][0]; j--) {
			double dist_diff;
			short closer_station = GetCloserStation(house[j], i, station, dist_diff);
			if (closer_station == alloc[j]) break;

			change_candi[idx++] = { j, alloc[j], closer_station, dist_diff };
		}
		for (int j = (changable_area[i][0] + changable_area[i][1]) / 2 + 1; j < changable_area[i][1]; j++) {
			double dist_diff;
			short closer_station = GetCloserStation(house[j], i, station, dist_diff);
			if (closer_station == alloc[j]) break;

			change_candi[idx++] = { j, alloc[j], closer_station, dist_diff };
		}
	}
	return idx;
}
int MedianOfThree(ChangeInfo change_candi[], int low, int mid, int high) {
	double a = change_candi[low].dist_diff, b = change_candi[mid].dist_diff, c = change_candi[high].dist_diff;

	if (a <= b && b <= c || c <= b && b <= a) return mid;
	else if (b <= a && a <= c || c <= a && a <= b) return low;
	else return high;
}
void Swap(ChangeInfo& a, ChangeInfo& b) {
	ChangeInfo tmp = { a.idx, a.from, a.to, a.dist_diff };
	a = { b.idx, b.from, b.to, b.dist_diff };
	b = { tmp.idx, tmp.from, tmp.to, tmp.dist_diff };
}
int Partition(ChangeInfo arr[], int low, int high) {
	int mid = (low + high) / 2;
	int pivot_index = MedianOfThree(arr, low, mid, high);
	Swap(arr[pivot_index], arr[high]);
	pivot_index = high;
	double pivot = arr[pivot_index].dist_diff;

	high--;
	while (1) {
		while (arr[low].dist_diff <= pivot) low++;
		while (arr[high].dist_diff >= pivot) high--;

		if (low >= high) break;
		else {
			Swap(arr[low], arr[high]);
			low++;
		}
	}
	Swap(arr[low], arr[pivot_index]);
	return low;
}
void QuickSort(ChangeInfo change_candi[], int low, int high) {
	if (low < high) {
		int pt = Partition(change_candi, low, high);

		QuickSort(change_candi, low, pt - 1);
		QuickSort(change_candi, pt + 1, high);
	}
}
bool ChangeGroup(const double house[], double station[], short alloc[]) {
	// 바꿀 house들을 우선 찾기만 한 뒤, score감소가 큰 놈부터 앞에 오게 정렬 후, 앞에서부터 바꾸기
	bool group_changed = false;
	int candi_size = FindChangableCandi(house, station, alloc, changable_candi);
	QuickSort(changable_candi, 0, candi_size - 1);
	for (int i = 0; i < candi_size; i++) {
		if (alloc_num[changable_candi[i].to] >= MAX_ALLOC) continue;
		alloc_num[changable_candi[i].to]++;
		alloc_num[changable_candi[i].from]--;
		alloc[changable_candi[i].idx] = changable_candi[i].to;
		group_changed = true;
	}
	return group_changed;
}

void test(const double house[], double station[], short alloc[]) {
	// 0. house 정렬
	for (int i = 0; i < HOUSE_NUM; i++) {
		shouse[i] = house[i];
		sindex[i] = i;
	}
	QuickSort(shouse, sindex, 0, HOUSE_NUM - 1);
	
	// 1. 왼쪽부터 차례대로 1/N씩 끊어서 그룹 형성
	Init(salloc);

	while (1) {
		// 2. 그룹의 평균에 station 위치시키기
		AllocStation2Avg(shouse, salloc, station);
		debug::PrintStatus(station);


		// 3. 경계선에 있는 house들 그룹 바꿀껀지 결정
		bool group_changed = ChangeGroup(shouse, station, salloc);
		debug::PrintStatus(station);
		if (!group_changed) break;
	}

	// 4. sindex에 따라 alloc 할당하기
	for (int i = 0; i < HOUSE_NUM; i++) {
		alloc[sindex[i]] = salloc[i];
	}
}

/*
* house 개수는 10만개
* station 개수는 5개
* 하나의 station에 할당된 house가 22'000개를 넘으면 안댐	<< 이게 핵심같음. 만약 20'000개였다면 문제가 O(N)만에 끝났을 듯
* house와 station의 거리가 SCORE
* 
* 1. station을 어디에 위치시킬 것인가
* 2. 위치시킨 station들과 house를 어떻게 연결지을 것인가
* 
* [브루트 포스]
* 1. 왼쪽부터 차례대로 1/5씩 끊어서 그룹 형성
* 2. 그룹의 평균에 station 위치시키기
*	- 집단을 바꾸고 station을 재위치 시켰을 때와, 그 이전을 비교하여서 더 점수가 안좋아졌다면 그만 break	<< 이런 경우의 수가 있나? 크게 생각이 안난다. 한번 확인해볼 필요가
* 3. 경계선에 있는 house들 그룹 바꿀껀지 결정
*	- 바꿀 house들을 우선 찾기만 한 뒤, score감소가 큰 놈부터 앞에 오게 정렬 후, 앞에서부터 바꾸기
*	2번 3번 반복 untill 더이상 그룹이 바뀌지 않을 때까지
*/

/*
* [브루트 포스] 3시간 20분
* seed: 5 - score: 518'842.134353
*/
