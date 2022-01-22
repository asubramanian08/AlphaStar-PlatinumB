#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;
#define MAX_SIDE 500

// DSU or union-find
class DSU
{
private:
    int *id;
    int *sz;

public:
    DSU(int n_ele = MAX_SIDE);
    // find its first parent
    int root(int idx);
    bool is_connected(int n1, int n2);
    void connect(int n1, int n2);
    int compSize(int idx);
    ~DSU();
};
DSU::DSU(int n_ele)
{
    id = new int[n_ele];
    sz = new int[n_ele];
    for (int i = 0; i < n_ele; i++)
    {
        id[i] = i;
        sz[i] = 1;
    }
}
int DSU::root(int idx)
{
    while (idx != id[idx])
    {
        id[idx] = id[id[idx]];
        idx = id[idx];
    }
    return idx;
}
bool DSU::is_connected(int n1, int n2) { return root(n1) == root(n2); }
void DSU::connect(int n1, int n2)
{
    int i = root(n1), j = root(n2);
    if (i == j)
        return;
    if (sz[i] < sz[j])
    {
        id[i] = j;
        sz[j] += sz[i];
    }
    else
    {
        id[j] = i;
        sz[i] += sz[j];
    }
}
int DSU::compSize(int idx) { return sz[root(idx)]; }
DSU::~DSU()
{
    delete[] id;
    delete[] sz;
}

int main(void)
{
    // read
    int rows, cols, range;
    cin >> rows >> cols >> range;
    auto getPos = [cols](int r, int c)
    { return r * cols + c; };
    int height[MAX_SIDE * MAX_SIDE];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            cin >> height[getPos(i, j)];
    int startCt[MAX_SIDE * MAX_SIDE] = {0};
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            cin >> startCt[getPos(i, j)];

    // setup: dsu + edges
    using pii = pair<int, int>;
    vector<pii> edges;
    DSU dsu(rows * cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            if (i > 0)
                edges.push_back(pii(getPos(i, j), getPos(i - 1, j)));
            if (j > 0)
                edges.push_back(pii(getPos(i, j), getPos(i, j - 1)));
        }
    sort(edges.begin(), edges.end(), [&height](pii e1, pii e2)
         { return abs(height[e1.first] - height[e1.second]) <
                  abs(height[e2.first] - height[e2.second]); });

    // connect edges
    long long ans = 0, starts;
    for (pii edge : edges)
    {
        if (dsu.is_connected(edge.first, edge.second))
            continue;
        starts = startCt[dsu.root(edge.first)] + startCt[dsu.root(edge.second)];
        dsu.connect(edge.first, edge.second);
        if (starts && dsu.compSize(edge.first) >= range)
        {
            ans += starts * abs(height[edge.first] - height[edge.second]);
            startCt[dsu.root(edge.first)] = 0;
        }
        else
            startCt[dsu.root(edge.first)] = starts;
    }

    // print
    cout << ans << endl;
    return 0;
}