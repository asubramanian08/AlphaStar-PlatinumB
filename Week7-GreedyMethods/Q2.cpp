#include <iostream>
#include <algorithm>
using namespace std;
using pii = pair<int, int>;
#define NA 1'000'000'000

int main(void)
{
    // get relevent covers
    int corral, coverCt;
    cin >> corral >> coverCt;
    pii *covers = new pii[coverCt];
    for (int i = 0; i < coverCt; i++)
        cin >> covers[i].first >> covers[i].second;
    sort(covers, covers + coverCt);
    int currEnd = 0, newCoverCt = coverCt;
    for (int i = 0; i < coverCt; i++)
        if (covers[i].first + covers[i].second <= currEnd)
            covers[i].first = NA, newCoverCt--;
        else
            currEnd = covers[i].first + covers[i].second;
    sort(covers, covers + coverCt);
    coverCt = newCoverCt;

    // next cover
    int *nextCover = new int[coverCt];
    for (int i = coverCt - 1, farthest = coverCt - 1; i >= 0; i--)
    {
        while (farthest >= 0 && covers[farthest].first > covers[i].first + covers[i].second)
            farthest--;
        nextCover[i] = farthest;
    }

    // binary lifting - use other nodes finish times
    int *finish = new int[coverCt], ans = NA;
    for (int start = coverCt - 1; covers[start].first + covers[start].second >= corral; start--)
    {
        int end = covers[start].first + covers[start].second - corral;
        int finishDist = covers[start].first - end;
        if (finishDist <= 0) // nothing to finish
        {
            ans = 1;
            break;
        }
        finish[start] = 1;
        for (int i = start - 1; i >= 0; i--)
        {
            finish[i] = finish[nextCover[i]] + 1;
            if (covers[i].first <= end)
            {
                ans = min(ans, finish[i]);
                break;
            }
        }
    }

    // print and delete
    cout << ans << endl;
    delete[] covers;
    delete[] nextCover;
    delete[] finish;
    return 0;
}