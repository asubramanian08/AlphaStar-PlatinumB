#include <iostream>
#include <algorithm>
using namespace std;

int main(void)
{
    // read
    long long vertRange, horzRange, vert, horz;
    cin >> vertRange >> horzRange >> vert >> horz;
    int *vertSize = new int[vert + 1];
    for (int i = 0; i < vert; i++)
        cin >> vertSize[i];
    vertSize[vert] = vertRange;
    sort(vertSize, vertSize + vert);
    for (int i = 0, last = 0; i <= vert; last += vertSize[i], i++)
        vertSize[i] -= last;
    sort(vertSize, vertSize + vert + 1);
    int *horzSize = new int[horz + 1];
    for (int i = 0; i < horz; i++)
        cin >> horzSize[i];
    horzSize[horz] = horzRange;
    sort(horzSize, horzSize + horz);
    for (int i = 0, last = 0; i <= horz; last += horzSize[i], i++)
        horzSize[i] -= last;
    sort(horzSize, horzSize + horz + 1);

    // choose the best fences to break down
    long long removed = vertSize[0] * horz + horzSize[0] * vert;
    int vertIdx = 1, horzIdx = 1;
    while (vertIdx <= vert && horzIdx <= horz)
        if (vertSize[vertIdx] < horzSize[horzIdx])
            removed += (horz - horzIdx + 1) * vertSize[vertIdx++];
        else //(vertSize[vertIdx] >= horzSize[horzIdx])
            removed += (vert - vertIdx + 1) * horzSize[horzIdx++];

    // print and delete
    cout << removed << endl;
    delete[] vertSize;
    delete[] horzSize;
    return 0;
}