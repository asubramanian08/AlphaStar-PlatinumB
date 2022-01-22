#include <iostream>
#include <vector>
using namespace std;
#define MAX_SIDE 1000
#define MOD 1000000007
#define NA -1

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

vector<int> children[MAX_SIDE * MAX_SIDE];
// DP[node] = (∏ DP[children]) + 1
long long possibleFills(int pos)
{
    long long ans = 1;
    for (int next : children[pos])
        ans = ans * possibleFills(next) % MOD;
    return ans + 1;
}

int main(void)
{
    // read
    int rows, cols;
    cin >> rows >> cols;
    bool grid[MAX_SIDE][MAX_SIDE];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            char ch;   // true = empty
            cin >> ch; // false = block
            grid[i][j] = ch == '.';
        }

    // setup: dsu ...
    DSU *dsu = new DSU[rows];
    int parent[MAX_SIDE][MAX_SIDE];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            parent[i][j] = NA;

    // parent and connets row with dsu
    for (int i = rows - 2; i > 0; i--)
        for (int j = 1; j < cols; j++)
            if (grid[i][j])
            {
                if (grid[i][j - 1])
                    dsu[i].connect(j, j - 1);
                if (grid[i + 1][j])
                    if (parent[i + 1][dsu[i + 1].root(j)] == NA)
                        parent[i + 1][dsu[i + 1].root(j)] = j;
                    else
                        dsu[i].connect(j, parent[i + 1][dsu[i + 1].root(j)]);
            }

    // parent tree
    auto findPos = [cols](int r, int c)
    { return r * cols + c; };
    for (int i = rows - 1; i > 0; i--)
        for (int j = 0; j < cols; j++)
            if (grid[i][j] && dsu[i].root(j) == j)
            {
                if (parent[i][j] != NA)
                    parent[i][j] = findPos(i - 1, dsu[i - 1].root(parent[i][j]));
                else
                    parent[i][j] = 0;
                children[parent[i][j]].push_back(findPos(i, j));
            }

    // print and delete
    cout << (possibleFills(0) - 1) << endl;
    return 0;
}