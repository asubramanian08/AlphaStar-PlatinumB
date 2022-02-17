// Time limit on cases 9 and 10 (8/10)
#include <iostream>
using namespace std;
#define MAX_COWS 1010
#define MAX_DOORS 10
#define BIG_NUM 1000000000000

int main(void)
{
    // read
    int cowCt, doors;
    cin >> cowCt >> doors;
    long long *entering = new long long[2 * cowCt];
    for (int i = 0; i < cowCt; i++)
        cin >> entering[i];

    // process for every start
    long long DP[MAX_COWS][MAX_DOORS], ans = BIG_NUM;
    for (int start = 0; start < cowCt; start++)
    {
        long long *cows = entering + start;
        for (int i = 0; i <= cowCt + 1; i++)
            for (int j = 0; j <= doors + 1; j++)
                DP[i][j] = BIG_NUM;
        DP[cowCt][0] = 0;
        for (int i = 1; i <= doors; i++)
            for (int j = 0; j < cowCt; j++)
                for (long long k = j + 1, cost = 0; k <= cowCt; k++)
                {
                    cost += (k - j - 1) * cows[k - 1];
                    DP[j][i] = min(DP[j][i], cost + DP[k][i - 1]);
                }
        cows[cowCt] = cows[0];
        ans = min(ans, DP[0][doors]);
    }

    // print and delete
    cout << ans << endl;
    delete[] entering;
    return 0;
}