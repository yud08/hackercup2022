//hackercup.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <deque>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm> 
#include <ctime>
#include <cmath>
using namespace std;

void updatevalue(vector< map<int, int> >& fenwickTree, vector<int>& A, int X, int Y, int N) {
    //cout << "fc_____________ " << N << endl;
    int oX = X;
    while (X <= N) {
        //cout << X << endl;
        fenwickTree[X][Y]++;
        fenwickTree[X][A[oX - 1]]--;
        X += X & -X;
    }
    A[oX - 1] = Y;
}

map<int, int> returnmap(vector< map<int, int> >& fenwickTree, int K) {
    map<int, int> s;
    while (K >= 1) {
        for (auto p : fenwickTree[K])
            s[p.first] += p.second;
        K -= (K & -K);
    }
    return s;
}

map<int, int> getmap(vector<map<int, int>>& fenwickTree, int L, int R) {
    //cout << "L is " << L << " | R is " << R << endl;
    map<int, int> first = returnmap(fenwickTree, R);
    //if (L == 0)
    //    cout << "wth" << endl;
    if (L != -1 && L != 0) {
        map<int, int> second = returnmap(fenwickTree, L);
        for (auto p1 : second) {
            first[p1.first] -= p1.second;
        }
    }
    return first;
}

bool comp(map<int, int>& map1, map<int, int>& map2, int wanted) {
    map<int, bool> visited;
    int total = 0;
    for (auto p1 : map1) {
        visited[p1.first] = true;
        if (map2.find(p1.first) == map2.end()) {
            total += p1.second;
        }
        else {
            total += abs(p1.second - map2[p1.first]);
        }
    }
    for (auto p2 : map2) {
        if (visited[p2.first])
            continue;
        visited[p2.first] = true;
        if (map1.find(p2.first) == map1.end()) {
            total += p2.second;
        }
        else {
            total += abs(p2.second - map1[p2.first]);
        }
    }
    //cout << "total is " << total << endl;
    return total == wanted;
}

map<int, int> getmapOLD(vector<map<int, int>>& prefixSum, int L, int R) {
    map<int, int> first = prefixSum[R];
    //cout << L << " " << R << endl;
    if (L >= 0) {
        for (auto p1 : prefixSum[L]) {
            first[p1.first] -= p1.second;
        }
    }
    return first;
}

int highestPowerOf2(int n) {
    return (n & (~(n - 1)));
}

int main()
{
    ifstream inputfile("C:\\Users\\duyn2\\source\\repos\\hackercup\\INPUT\\perfectly_balanced_chapter_2_input.txt");
    ofstream outputfile("C:\\Users\\duyn2\\source\\repos\\hackercup\\OUTPUT\\output31.txt");
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    clock_t timereq = clock();
    int T;
    inputfile >> T;
    for (int i = 1; i <= T; i++) {
        cout << "TEST CASE: " << i << endl;
        int N;
        inputfile >> N;
        vector<int> A;
        for (int j = 0; j < N; j++) {
            int Ai;
            inputfile >> Ai;
            A.push_back(Ai);
        }
        vector<map<int, int>> prefixSum(N);
        map<int, int> curr;
        for (int x = 0; x < N; x++) {
            curr[A[x]]++;
            prefixSum[x] = curr;
        }
        vector< map<int, int> > fenwickTree(N + 1);
        for (int y = 1; y < N + 1; y++) {
            int range = highestPowerOf2(y);
            fenwickTree[y] = getmapOLD(prefixSum, y - range - 1, y - 1);
        }
        int Q;
        inputfile >> Q;
        cout << "Sizeof N: " << N << " and sizeof Q: " << Q << endl;
        int ans = 0;
        for (int z = 0; z < Q; z++) {
            int type, L, R;
            inputfile >> type;
            inputfile >> L;
            inputfile >> R;
            if (type == 1) {
                updatevalue(fenwickTree, A, L, R, N);
            }
            else {
                //cout << L << " " << R << endl;
                int sQ = R - L + 1;
                if (sQ & 1) {
                    bool firstodd = false;
                    bool exitnow = false;
                    map<int, int> first = getmap(fenwickTree, L - 1, R);
                    for (auto p2 : first) {
                        if (p2.second & 1) {
                            if (firstodd) {
                                exitnow = true;
                                break;
                            }
                            else
                                firstodd = true;
                        }
                    }
                    if (exitnow)
                        continue;   
                    //check
                    int half = sQ / 2 + 1;
                    map<int, int> midl, midr;
                    midl = getmap(fenwickTree, L - 1, L + half - 1);
                    midr = getmap(fenwickTree, L + half - 2, R);
                    int midlint = A[(L + R) / 2 - 1];
                    if (comp(midl, midr, 0)) {
                        ans++;
                        //cout << "worked1" << endl;
                        continue;
                    }
                    midl[midlint]--;
                    if (comp(midl, midr, 1)) {
                        ans++;
                        //cout << "worked2" << endl;
                        continue;
                    }
                    midl[midlint]++;
                    midr[midlint]--;
                    if (comp(midl, midr, 1)) {
                        ans++;
                        //cout << "worked3" << endl;
                    }
                }
            }
        }
        outputfile << "Case #" << i << ": " << ans << endl;
    }
    cout << endl;
    cout << (clock() - timereq) << " milliseconds" << endl;
    cout << (clock() - timereq) / 1000 << " seconds" << endl;
    inputfile.close();
    outputfile.close();
    return 0;
}