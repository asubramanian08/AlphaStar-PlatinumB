#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
#define MAX_SIDE 1505
#define DIRS 4

// Biconnected components
#define NA -1
using pii = pair<int, int>;
int *low, *vis;
vector<int> *component;
int added, numComps;
vector<int> *graph;
void DFS_BCC(int node, int par, vector<pii> &edges)
{
    low[node] = vis[node] = ++added;
    int children = 0;
    for (int next : graph[node])
        if (vis[next] == NA)
        {
            edges.push_back(pii(node, next));
            children++;
            DFS_BCC(next, node, edges);
            low[node] = min(low[node], low[next]);
            if ((par == NA ? children > 1 : low[next] >= vis[node]) && ++numComps)
                for (pii rEdge(node, next), prev(NA, NA); prev != rEdge; prev = edges.back(), edges.pop_back())
                {
                    component[edges.back().first].push_back(numComps - 1);
                    component[edges.back().second].push_back(numComps - 1);
                }
        }
        else if (next != par)
        {
            low[node] = min(low[node], vis[next]);
            if (vis[next] < vis[node])
                edges.push_back(pii(node, next));
        }
}
/* return number of components
 * extra param: preset the graph */
int biconnectedComponents(int numNodes)
{
    low = new int[numNodes];
    vis = new int[numNodes];
    component = new vector<int>[numNodes];
    added = numComps = 0;
    for (int i = 0; i < numNodes; i++)
        low[i] = vis[i] = NA;
    vector<pii> edges;
    for (int i = 0; i < numNodes; i++)
        if (vis[i] == NA)
        {
            DFS_BCC(i, NA, edges);
            if (!edges.empty())
                for (numComps++; !edges.empty(); edges.pop_back())
                {
                    component[edges.back().first].push_back(numComps - 1);
                    component[edges.back().second].push_back(numComps - 1);
                }
        }
    for (int i = 0; i < numNodes; i++)
    {
        sort(component[i].begin(), component[i].end());
        component[i].resize(unique(component[i].begin(), component[i].end()) - component[i].begin());
    }
    return numComps;
}
bool is_biconnected(int n1, int n2) // two pointer intersection
{
    int it1 = 0, it2 = 0;
    while (it1 < component[n1].size() && it2 < component[n2].size())
        if (component[n1][it1] < component[n2][it2])
            it1++;
        else if (component[n1][it1] > component[n2][it2])
            it2++;
        else
            return true;
    return false;
}

// Vars
int rows, cols, queries, numNodes = -1;
int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
char grid[MAX_SIDE][MAX_SIDE] = {0};
int convert[MAX_SIDE][MAX_SIDE];
bool checked[MAX_SIDE][MAX_SIDE] = {false};
bool DP[MAX_SIDE][MAX_SIDE][DIRS] = {false};
queue<pair<pii, int>> q;
auto addNode = [](pii b, int d)
{
    if (grid[b.first + 1][b.second + 1] && !DP[b.first][b.second][d] &&
        grid[b.first + dx[d] + 1][b.second + dy[d] + 1])
    {
        q.push(make_pair(b, d));
        DP[b.first][b.second][d] = true;
    }
};

// find vis starts
bool findStarts(pii pos, pii box)
{
    if (!grid[pos.first + 1][pos.second + 1] || checked[pos.first][pos.second] || pos == box)
        return pos == box;
    checked[pos.first][pos.second] = true;
    for (int d = 0; d < DIRS; d++)
        if (findStarts(pii(pos.first + dx[d], pos.second + dy[d]), box))
            addNode(box, (d + 2) % DIRS);
    return false;
}

int main(void)
{
    // read
    cin >> rows >> cols >> queries;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            cin >> grid[i + 1][j + 1];
            if (grid[i + 1][j + 1] == '#')
                grid[i + 1][j + 1] = 0;
            else
                convert[i][j] = ++numNodes;
        }

    // convert to graph, run BCC
    graph = new vector<int>[++numNodes];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (grid[i + 1][j + 1] != 0)
                for (int d = 0; d < DIRS; d++)
                    if (grid[i + dx[d] + 1][j + dy[d] + 1] != 0)
                        graph[convert[i][j]].push_back(convert[i + dx[d]][j + dy[d]]);
    biconnectedComponents(numNodes);

    // BFS and fill DP[r][c][d] = possible for box to be at r,c and bessie to its d direction
    pii bessie(NA, NA), box(NA, NA);
    for (int i = 0; i < rows && (bessie.first == NA || box.first == NA); i++)
        for (int j = 0; j < cols && (bessie.first == NA || box.first == NA); j++)
            if (grid[i + 1][j + 1] == 'A')
                bessie = pii(i, j);
            else if (grid[i + 1][j + 1] == 'B')
                box = pii(i, j);
    findStarts(bessie, box);
    while (!q.empty())
    {
        pii boxPos = q.front().first;
        int dir = q.front().second;
        q.pop();
        addNode(pii(boxPos.first - dx[dir], boxPos.second - dy[dir]), dir);
        for (int d = 0; d < DIRS; d++)
            if (grid[boxPos.first + dx[d] + 1][boxPos.second + dy[d] + 1] &&
                is_biconnected(convert[boxPos.first + dx[dir]][boxPos.second + dy[dir]],
                               convert[boxPos.first + dx[d]][boxPos.second + dy[d]]))
                addNode(boxPos, d);
    }

    // answer queries
    for (int i = 0; i < queries; i++)
    {
        int n1, n2;
        cin >> n1 >> n2;
        n1--, n2--;
        bool works = false;
        for (int d = 0; d < DIRS; d++)
            works |= DP[n1][n2][d];
        cout << (works || pii(n1, n2) == box ? "YES" : "NO") << endl;
    }
    return 0;
}