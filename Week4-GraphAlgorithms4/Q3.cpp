#include <iostream>
using namespace std;

#define MOD 1000000007

int main()
{
    // read
    int side, x, y;
    cin >> side;
    int *atPos = new int[side];
    for (int i = 0; i < side; i++)
    {
        cin >> x >> y;
        atPos[x] = y;
    }

    // left values
    int *left = new int[side];
    int low = side;
    for (int i = 0; i < side; i++)
        while (atPos[i] < low)
            left[--low] = i;
    while (low > 0)
        left[--low] = side;
    long long *sumLeft = new long long[side];
    sumLeft[side - 1] = left[side - 1];
    for (int j = side - 2; j >= 0; j--)
        sumLeft[j] = (sumLeft[j + 1] + left[j]) % MOD;
    long long *stairSumLeft = new long long[side];
    stairSumLeft[side - 1] = left[side - 1];
    for (int j = side - 2; j >= 0; j--)
        stairSumLeft[j] = (stairSumLeft[j + 1] + ((long long)left[j]) * (side - j)) % MOD;

    // top
    int *top = new int[side];
    top[side - 1] = atPos[side - 1];
    for (int i = side - 2; i >= 0; i--)
        top[i] = max(top[i + 1], atPos[i]);

    // calc ans
    long long ans = 0;
    int j = side - 1;
    for (int i = 1; i < side; i++)
    {
        while (j > 0 && left[j - 1] <= i)
            j--;
        long long bad = stairSumLeft[j] - stairSumLeft[top[i]] -
                        (side - top[i]) * (sumLeft[j] - sumLeft[top[i]]);
        bad = ((bad % MOD) + MOD) % MOD;
        long long bad2 = (top[i] * ((long long)j) - (j * ((long long)(j - 1))) / 2) % MOD;
        bad2 = (bad2 * i) % MOD;
        bad = (bad + bad2) % MOD;

        long long tot = ((top[i] * ((long long)(top[i] + 1))) / 2) % MOD;
        tot = (tot * i) % MOD;
        ans = ans + tot - bad;
        ans = ((ans % MOD) + MOD) % MOD;
    }

    // print and delete
    cout << ans << endl;
    delete[] atPos;
    delete[] left;
    delete[] sumLeft;
    delete[] stairSumLeft;
    delete[] top;
}