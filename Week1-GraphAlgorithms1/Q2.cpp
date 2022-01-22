#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main(void)
{
    // read
    int numCows, conns;
    cin >> numCows >> conns;
    int *talk = new int[numCows];
    for (int i = 0; i < numCows; i++)
        cin >> talk[i];
    using pii = pair<int, int>;
    vector<pii> *graph = new vector<pii>[numCows];
    for (int i = 0; i < conns; i++)
    {
        int n1, n2, tm;
        cin >> n1 >> n2 >> tm;
        n1--, n2--;
        graph[n1].push_back(pii(tm, n2));
        graph[n2].push_back(pii(tm, n1));
    }

    // smallest talker
    int src = 0;
    for (int i = 1; i < numCows; i++)
        if (talk[i] < talk[src])
            src = i;

    // mst algo on smallest talk
    priority_queue<pii, vector<pii>, greater<pii> > pq;
    pq.push(pii(talk[src], src));
    bool *vis = new bool[numCows];
    fill(vis, vis + numCows, false);
    int totalCost = 0;
    while (!pq.empty())
    {
        pii node = pq.top();
        pq.pop();
        if (vis[node.second])
            continue;
        vis[node.second] = true;
        totalCost += node.first;
        for (pii next : graph[node.second])
            pq.push(pii(talk[node.second] + 2 * next.first + talk[next.second], next.second));
    }

    // print and delete
    cout << totalCost << endl;
    delete[] talk;
    delete[] graph;
    delete[] vis;
    return 0;
}