#include <iostream>
using namespace std;
#define MAX_SIZE 7505

long long len, mod, nmod, DP[MAX_SIZE][MAX_SIZE];
long long fp(long long a, long long b) { return b <= 2 ? (b ? (b == 2 ? a * a % mod : a) : 1)
                                                       : (fp(fp(a, b / 2), 2) * (b % 2 ? a : 1) % mod); }
long long with(long long fact)
{
    long long ret = 0;
    long long *dp = new long long[len / fact + 1];
    dp[0] = nmod - 1;
    for (int i = 1; i <= len / fact; ++i)
        for (int j = 1; j <= i; ++j)
            dp[i] = (dp[i] - DP[i * fact - 1][j * fact - 1] * dp[i - j] % nmod + nmod) % nmod;
    for (int i = 1; i <= len / fact; ++i)
        ret = (ret + DP[len][len - i * fact] * dp[i] % nmod) % nmod;
    return ret;
}

int main(void)
{
    // read
    cin >> len >> mod;
    nmod = mod - 1;
    for (int i = 0; i <= len; i++)
    {
        DP[i][0] = 1;
        for (int j = 1; j <= i; j++)
            DP[i][j] = (DP[i][j - 1] * (i - j + 1)) % nmod;
    }
    bool *comp = new bool[len + 1];
    long long ans = 1;
    for (int i = 2; i <= len; ++i)
        if (!comp[i]) // is prime
        {
            for (int j = 2 * i; j <= len; j += i)
                comp[j] = 1; // power
            for (int j = i; j <= len; j *= i)
                ans = ans * fp(i, with(j)) % mod;
        }
    delete[] comp;
    cout << ans << endl;
    return 0;
}