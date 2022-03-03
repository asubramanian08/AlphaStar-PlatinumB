#include <iostream>
#include <cmath>
using namespace std;
#define MAX_TIME 1e18
#define PRECISION_RUNS 500

long long usedCows(long double benifit, long long *work, long long floors)
{
    long long working = 0;
    for (int i = 0; i < floors; i++)
        // Solve: work[i] / x * (x + 1) >= benifit
        working += (long long)((sqrt(1 + 4 * work[i] / benifit) - 1) / 2);
    return working;
}

int main(void)
{
    long long floors, workers;
    cin >> floors >> workers;
    workers -= floors; // one for every floor
    long long *work = new long long[floors];
    for (int i = 0; i < floors; i++)
        cin >> work[i];
    long double lo = 0, hi = MAX_TIME, mid;
    for (int runs = 0; runs < PRECISION_RUNS; runs++)
        if (mid = (lo + hi) / 2; usedCows(mid, work, floors) >= workers)
            lo = mid;
        else
            hi = mid;
    long double ans = 0;
    long long tot = 0;
    for (int i = 0; i < floors; i++)
    {
        long long x = (sqrt(1 + 4 * work[i] / lo) - 1) / 2;
        ans += 1.0 * work[i] / (x + 1), tot += x;
    }
    cout << (long long)round(ans - (workers - tot) * lo) << endl;
    delete[] work;
    return 0;
}