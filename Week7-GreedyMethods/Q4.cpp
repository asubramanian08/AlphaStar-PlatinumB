#include <iostream>
#include <set>
using namespace std;

int main(void)
{
    // read
    int cardCt;
    cin >> cardCt;
    int *ecards = new int[cardCt];
    for (int i = 0; i < cardCt; i++)
        cin >> ecards[i];
    bool *isElsie = new bool[cardCt * 2];
    for (int i = 0; i < cardCt * 2; i++)
        isElsie[i] = false;
    for (int i = 0; i < cardCt; i++)
        isElsie[ecards[i] - 1] = true;
    int *bcards = new int[cardCt];
    for (int i = 0, bpos = -1; i < 2 * cardCt; i++)
        if (!isElsie[i])
            bcards[++bpos] = i;

    // high card
    bool *highWin = new bool[cardCt];
    set<int> bhand;
    for (int i = 0; i < cardCt; i++)
        bhand.insert(bcards[i]), highWin[i] = false;
    for (int i = 0; i < cardCt; i++)
        if (auto play = bhand.lower_bound(ecards[i]); play != bhand.end())
            highWin[i] = true, bhand.erase(play);

    // low card
    bool *lowWin = new bool[cardCt];
    bhand.clear();
    for (int i = 0; i < cardCt; i++)
        bhand.insert(bcards[i]), lowWin[i] = false;
    for (int i = cardCt - 1; i >= 0; i--)
        if (auto play = bhand.lower_bound(ecards[i]); play != bhand.begin())
            lowWin[i] = true, bhand.erase(--play);

    // {i|0≤i≤cardCt} max ( ∑_{j=0}^{i} (highWin[j]) + ∑_{j=i+1}^{cardCt} (lowWin[j]) )
    int highCt = 0, lowCt = 0;
    for (int i = 0; i < cardCt; i++)
        lowCt += lowWin[i];
    int ans = highCt + lowCt;
    for (int i = 0; i < cardCt; i++)
    {
        highCt += highWin[i];
        lowCt -= lowWin[i];
        ans = max(ans, highCt + lowCt);
    }

    // print and delete
    cout << ans << endl;
    delete[] ecards;
    delete[] isElsie;
    delete[] bcards;
    delete[] highWin;
    delete[] lowWin;
    return 0;
}