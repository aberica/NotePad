#include <iostream>
#include <vector>
#define MAX_N 1010
using namespace std;

int T, N;
int line[MAX_N][2];

void Input() {
    cin >> N;
    
    for (int from = 0; from < N; from++) {
        line[from][0] = line[from][1] = 0;
    }

    for (int from = 0; from < N; from++) {
        for (int to = from+1; to < N; to++) {
            int color; cin >> color;
            line[from][color]++;
            line[to][color]++;
        }
    }
}
int CalCnt() {
    int cnt = 0;
    for (int from = 0; from < N; from++) {
        cnt += line[from][0] * line[from][1];
    }
    int all_cases = N * (N-1) * (N-2) / 6;
    return all_cases - cnt / 2;
}

int main() {
    cin >> T;
    while(T--) {
        Input();
        cout << CalCnt() << endl;
    }
    return 0;
}
