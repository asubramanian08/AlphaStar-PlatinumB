#include <iostream>
#include <algorithm>
using namespace std;
#define NA -1

void graze(int &grazeSz, int &grazeGang, int newGang)
{
    grazeSz ? (grazeGang == newGang ? grazeSz++ : grazeSz--)
            : (grazeSz = 1, grazeGang = newGang);
}

int maxRemaining(int *gangSz, int gangCt, int grazeSz = 0, int grazeGang = NA)
{
    int *currGang = new int[gangCt];
    copy(gangSz, gangSz + gangCt, currGang);
    sort(currGang + 1, currGang + gangCt);
    while (currGang[gangCt - 1] > 0)
        for (int i = gangCt - 1; i > 0; i--)
        {
            graze(grazeSz, grazeGang, i);
            currGang[i]--;
            if (currGang[i] >= currGang[i - 1])
                break;
        }
    for (int i = 0; i < currGang[0]; i++)
        graze(grazeSz, grazeGang, 0);
    delete[] currGang;
    return grazeGang == 0 ? grazeSz : NA;
}

int main(void)
{
    int cowCt, gangCt;
    cin >> cowCt >> gangCt;
    int *gangSz = new int[gangCt];
    for (int i = 0; i < gangCt; i++)
        cin >> gangSz[i];
    int ans = maxRemaining(gangSz, gangCt);
    cout << (ans <= 0 ? "NO" : "YES") << endl;
    if (ans <= 0)
        return 0;
    else
        cout << ans << endl;
    int grazeSz = 0, grazeGang = NA, prev_gs, prev_gg;
    for (int i = 0, j; i < cowCt; i++)
        for (j = 0; j < gangCt; j++)
            if (gangSz[j] > 0)
            {
                prev_gs = grazeSz, prev_gg = grazeGang;
                graze(grazeSz, grazeGang, j);
                gangSz[j]--;
                if (maxRemaining(gangSz, gangCt, grazeSz, grazeGang) == ans)
                {
                    cout << (j + 1) << endl;
                    break;
                }
                grazeSz = prev_gs, grazeGang = prev_gg;
                gangSz[j]++;
            }
    delete[] gangSz;
    return 0;
}