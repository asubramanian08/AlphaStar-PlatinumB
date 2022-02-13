#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Strongly Connected Components
#define NA -1
void TopologicalOrder(int node, vector<int> *graph, vector<int> &order, bool *vis)
{
    if (vis[node])
        return;
    vis[node] = true;
    for (int next : graph[node])
        TopologicalOrder(next, graph, order, vis);
    order.push_back(node);
}
void DFS_component(int node, vector<int> *graph, int *comp, int currComp)
{
    if (comp[node] != NA)
        return;
    comp[node] = currComp;
    for (int next : graph[node])
        DFS_component(next, graph, comp, currComp);
}
int *SCC(vector<int> *graph, int numNodes)
{
    vector<int> order;
    bool *vis = new bool[numNodes];
    fill(vis, vis + numNodes, false);
    for (int i = 0; i < numNodes; i++)
        TopologicalOrder(i, graph, order, vis);
    reverse(order.begin(), order.end());
    vector<int> *rgraph = new vector<int>[numNodes];
    for (int i = 0; i < numNodes; i++)
        for (int next : graph[i])
            rgraph[next].push_back(i);
    int *component = new int[numNodes];
    fill(component, component + numNodes, NA);
    int compCt = -1;
    for (int i = 0; i < numNodes; i++)
        if (component[order[i]] == NA)
            DFS_component(order[i], rgraph, component, ++compCt);
    delete[] vis;
    delete[] rgraph;
    return component;
}

int main(void)
{
    // read
    int numNodes, conns;
    cin >> numNodes >> conns;
    vector<int> *graph = new vector<int>[numNodes];
    for (int i = 0; i < conns; i++)
    {
        int n1, n2;
        cin >> n1 >> n2;
        graph[n1 - 1].push_back(n2 - 1);
    }

    // SCC + count multinode components
    int *component = SCC(graph, numNodes);
    int numComps = 0;
    for (int i = 0; i < numNodes; i++)
        numComps = max(numComps, component[i] + 1);
    int *compSz = new int[numComps];
    fill(compSz, compSz + numComps, 0);
    for (int i = 0; i < numNodes; i++)
        compSz[component[i]]++;
    int compCt = 0;
    for (int i = 0; i < numComps; i++)
        compCt += compSz[i] > 1;

    // print and delete
    cout << compCt << endl;
    delete[] graph;
    delete[] component;
    delete[] compSz;
    return 0;
}