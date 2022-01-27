#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

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

int DFS(int node, int *flows, int *toAdd, vector<int> *graph, int parent)
{
    for (int next : graph[node])
        if (next != parent)
            toAdd[node] += DFS(next, flows, toAdd, graph, node);
    flows[node] += toAdd[node];
    return toAdd[node];
}

int main(void)
{
    int nodes, pumps, n1, n2, ancestor;
    cin >> nodes >> pumps;
    vector<int> *graph = new vector<int>[nodes];
    for (int i = 1; i < nodes; i++)
    {
        cin >> n1 >> n2;
        graph[n1 - 1].push_back(n2 - 1);
        graph[n2 - 1].push_back(n1 - 1);
    }
    LCA lca(graph, nodes);
    int *toAdd = new int[nodes];
    int *flows = new int[nodes];
    for (int i = 0; i < nodes; i++)
        toAdd[i] = flows[i] = 0;
    for (int i = 0; i < pumps; i++)
    {
        cin >> n1 >> n2;
        ancestor = lca.query(n1 - 1, n2 - 1);
        toAdd[n1 - 1]++;
        toAdd[n2 - 1]++;
        toAdd[ancestor] -= 2;
        flows[ancestor]++;
    }
    DFS(0, flows, toAdd, graph, NA);
    int largest = 0;
    for (int i = 0; i < nodes; i++)
        largest = max(largest, flows[i]);
    cout << largest << endl;
    delete[] graph;
    delete[] toAdd;
    delete[] flows;
    return 0;
}