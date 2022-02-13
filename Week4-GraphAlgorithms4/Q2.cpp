// 10/14 -> SCC is implemented wrong
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Strongly Connected Components -> wrong implementation
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

void setDist(const vector<int> &order, vector<int> *graph, int numNodes, int *dist, int *size)
{
    for (int node : order)
        for (int next : graph[node])
            dist[next] = max(dist[node], dist[node] + size[next]);
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

    // graph representation of SCC
    int *component = SCC(graph, numNodes);
    for (int i = 0; i < numNodes; i++)
        cin >> component[i];
    int numComps = 0;
    for (int i = 0; i < numNodes; i++)
        numComps = max(numComps, component[i] + 1);
    vector<int> *SCCgraph = new vector<int>[numComps];
    vector<int> *rSCCgraph = new vector<int>[numComps];
    for (int i = 0; i < numNodes; i++)
        for (int next : graph[i])
            if (component[i] != component[next])
            {
                SCCgraph[component[i]].push_back(component[next]);
                rSCCgraph[component[next]].push_back(component[i]);
            }
    for (int i = 0; i < numComps; i++)
    {
        sort(SCCgraph[i].begin(), SCCgraph[i].end());
        SCCgraph[i].resize(unique(SCCgraph[i].begin(), SCCgraph[i].end()) - SCCgraph[i].begin());
    }

    // maximum visited from 0 -> v and v -> 0
    int *compSz = new int[numComps];
    fill(compSz, compSz + numComps, 0);
    for (int i = 0; i < numNodes; i++)
        compSz[component[i]]++;
    int *dist = new int[numComps];
    fill(dist, dist + numComps, NA);
    vector<int> downOrder, upOrder;
    bool *vis = new bool[numComps];
    fill(vis, vis + numComps, false);
    TopologicalOrder(component[0], SCCgraph, downOrder, vis);
    fill(vis, vis + numComps, false);
    TopologicalOrder(component[0], rSCCgraph, upOrder, vis);
    reverse(downOrder.begin(), downOrder.end());
    reverse(upOrder.begin(), upOrder.end());
    dist[downOrder.front()] = dist[upOrder.front()] = 0;
    setDist(downOrder, SCCgraph, numNodes, dist, compSz);
    setDist(upOrder, rSCCgraph, numNodes, dist, compSz);

    // max dist - find backwards edge
    bool *type = new bool[numComps];
    fill(type, type + numComps, false);
    for (int node : upOrder)
        type[node] = true;
    int ans = compSz[component[0]];
    for (int i = 0; i < numComps; i++)
        if (dist[i] != NA)
            for (int next : SCCgraph[i])
                if (dist[next] != NA && (type[i] != type[next] || i == component[0] || next == component[0]))
                    ans = max(ans, dist[next] + dist[i] + compSz[component[0]]);

    // print and delete
    cout << ans << endl;
    delete[] graph;
    delete[] component;
    delete[] SCCgraph;
    delete[] rSCCgraph;
    delete[] compSz;
    delete[] dist;
    delete[] vis;
    delete[] type;
    return 0;
}