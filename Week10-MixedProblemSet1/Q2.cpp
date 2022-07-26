#include <iostream>
#include <vector>
using namespace std;

int main(void)
{
    // read
    int shelfCt, maxWidth;
    cin >> shelfCt >> maxWidth;
    using pii = pair<int, int>;
    pii *shelves = new pii[shelfCt];
    for (int i = 0; i < shelfCt; i++)
        cin >> shelves[i].first >> shelves[i].second;

    // prefix sum on widths
    int *widthPS = new int[shelfCt];
    widthPS[0] = shelves[0].second;
    for (int i = 1; i < shelfCt; i++)
        widthPS[i] = widthPS[i - 1] + shelves[i].second;

    // sliding window - shelf start, tallest book
    vector<int> tallest; // position (height)
}