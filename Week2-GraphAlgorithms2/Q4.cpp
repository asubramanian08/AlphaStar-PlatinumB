#include <iostream>
#include <vector>
#include <cmath>
#include <map>
using namespace std;
#define MAX_FIELDS 200000

// Least common ancestor -> binary lifting
#define NA -1
class LCA
{
private:
    int **sparseTBL, *dpt, length, log;
    vector<int> *graph;
    void init(int node, int parent, int lev);

public:
    LCA(vector<int> *g, int len);
    ~LCA();
    int query(int node1, int node2);
    int depth(int node);
    int nextDown(int from, int to);
};
void LCA::init(int node, int parent, int lev)
{
    dpt[node] = lev;
    sparseTBL[node][0] = parent;
    for (int i = 1; (i <= log) && (sparseTBL[node][i - 1] != NA); i++)
        sparseTBL[node][i] = sparseTBL[sparseTBL[node][i - 1]][i - 1];
    for (int i = 0; i < graph[node].size(); i++)
        if (graph[node][i] != parent)
            init(graph[node][i], node, lev + 1);
}
LCA::LCA(vector<int> *g, int len)
{
    graph = g, length = len, log = log2(len);
    sparseTBL = new int *[length];
    for (int i = 0; i < length; i++)
    {
        sparseTBL[i] = new int[log + 1];
        for (int j = 0; j <= log; j++)
            sparseTBL[i][j] = NA;
    }
    dpt = new int[length];
    init(0, NA, 0);
}
LCA::~LCA()
{
    for (int i = 0; i < length; i++)
        delete[] sparseTBL[i];
    delete[] sparseTBL;
    delete[] dpt;
}
int LCA::query(int node1, int node2)
{
    // fix nodes
    if (dpt[node1] < dpt[node2])
    {
        int temp = node1;
        node1 = node2;
        node2 = temp;
    }
    for (int i = log; i >= 0; i--) // loop though bits
        if (dpt[node1] - (1 << i) >= dpt[node2])
            node1 = sparseTBL[node1][i];
    if (node1 == node2) // n2 is ancestor
        return node2;

    // main compute
    for (int i = log; i >= 0; i--)
        if (sparseTBL[node1][i] != sparseTBL[node2][i])
        {
            node1 = sparseTBL[node1][i];
            node2 = sparseTBL[node2][i];
        }
    return sparseTBL[node1][0];
}
int LCA::depth(int node) { return dpt[node]; }
int LCA::nextDown(int from, int to)
{
    if (from == to)
        return NA;
    for (int j = log; j >= 0; j--)
        if (sparseTBL[to][j] != NA && dpt[from] < dpt[sparseTBL[to][j]])
            to = sparseTBL[to][j];
    return to;
}

void DFS(int node, int parent, int *ancestorPaths, vector<int> *tree, int *intersection)
{
    ancestorPaths[node] = (parent != NA ? ancestorPaths[parent] : 0) + intersection[node];
    for (int next : tree[node])
        if (next != parent)
            DFS(next, node, ancestorPaths, tree, intersection);
}

int main(void)
{
    // read
    int numNodes, numConns, numNons;
    cin >> numNodes >> numConns;
    numNons = numConns - (numNodes - 1);
    vector<int> *tree = new vector<int>[numConns];
    for (int i = 1; i < numNodes; i++)
    {
        int n1, n2;
        cin >> n1 >> n2;
        n1--, n2--;
        tree[n1].push_back(n2);
        tree[n2].push_back(n1);
    }
    using pii = pair<int, int>;
    pair<pii, int> *non_standard = new pair<pii, int>[numNons];
    for (int i = 0; i < numNons; i++)
    {
        cin >> non_standard[i].first.first >> non_standard[i].first.second;
        --non_standard[i].first.first, --non_standard[i].first.second;
    }

    // prosess non_standards
    LCA lca(tree, numNodes);
    int *intersection = new int[numNodes];
    map<pii, int> pairCount;
    long long ans = 0;
    for (int i = 0; i < numNons; i++)
    {
        non_standard[i].second = lca.query(non_standard[i].first.first, non_standard[i].first.second);
        int next1 = lca.nextDown(non_standard[i].second, non_standard[i].first.first);
        if (next1 != NA)
            ans -= ++intersection[next1];
        int next2 = lca.nextDown(non_standard[i].second, non_standard[i].first.second);
        if (next2 != NA)
            ans -= ++intersection[next2];
        if (next1 != NA && next2 != NA)
            ans -= ++pairCount[pii(max(next1, next2), min(next1, next2))] - 1;
    }

    // DFS and calculate answer
    int *ancestorPaths = new int[numNodes];
    DFS(0, NA, ancestorPaths, tree, intersection);
    for (int i = 0; i < numNons; i++)
        ans += ancestorPaths[non_standard[i].first.first] +
               ancestorPaths[non_standard[i].first.second] -
               2 * ancestorPaths[non_standard[i].second];

    // print and delete
    cout << ans << endl;
    delete[] tree;
    delete[] non_standard;
    delete[] intersection;
    delete[] ancestorPaths;
    return 0;
}