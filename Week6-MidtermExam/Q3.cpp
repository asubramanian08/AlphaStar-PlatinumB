#include <iostream>
#include <vector>
#include <queue>
using namespace std;
using pii = pair<int, int>;
#define BIG_NUM 1000000000
#define NA -1

int DFS(int node, vector<pii> *dTree, vector<pii> *unused, int *bestPath, int *cost)
{
    bestPath[node] = BIG_NUM;
    for (pii next : dTree[node])
        bestPath[node] = min(bestPath[node], DFS(next.second, dTree, unused, bestPath, cost) + next.first);
    for (pii next : unused[node])
        bestPath[node] = min(bestPath[node], cost[next.second] + next.first);
    return bestPath[node];
}

int main(void)
{
    // read
    int nodes, conns;
    cin >> nodes >> conns;
    vector<pii> *graph = new vector<pii>[nodes];
    for (int i = 0; i < conns; i++)
    {
        int n1, n2, wt;
        cin >> n1 >> n2 >> wt;
        n1--, n2--;
        graph[n1].push_back(pii(wt, n2));
        graph[n2].push_back(pii(wt, n1));
    }

    // dijkstra tree
    vector<pii> *dTree = new vector<pii>[nodes];
    vector<pii> *unused = new vector<pii>[nodes];
    priority_queue<pair<pii, int>, vector<pair<pii, int>>, greater<pair<pii, int>>> pq;
    int *cost = new int[nodes];
    for (int i = 0; i < nodes; i++)
        cost[i] = BIG_NUM;
    pq.push(pair(pii(0, 0), NA));
    while (!pq.empty())
    {
        int wt = pq.top().first.first, nd = pq.top().first.second, pr = pq.top().second;
        pq.pop();
        if (cost[nd] != BIG_NUM)
        {
            unused[nd].push_back(pii(wt - cost[pr], pr));
            unused[pr].push_back(pii(wt - cost[pr], nd));
            continue;
        }
        cost[nd] = wt;
        if (pr != NA)
            dTree[pr].push_back(pii(wt - cost[pr], nd));
        for (pii nx : graph[nd])
            if (nx.second != pr && cost[nx.second] == BIG_NUM)
                pq.push(pair(pii(wt + nx.first, nx.second), nd));
    }

    // DFS for the best path
    int *bestPath = new int[nodes];
    DFS(0, dTree, unused, bestPath, cost);

    // print and delete
    for (int i = 1; i < nodes; i++)
        cout << (bestPath[i] == BIG_NUM ? -1 : bestPath[i]) << endl;
    delete[] graph;
    delete[] dTree;
    delete[] unused;
    delete[] cost;
    delete[] bestPath;
    return 0;
}