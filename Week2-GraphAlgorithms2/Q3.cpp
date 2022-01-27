#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

using pii = pair<int, int>;
#define MOD 1000000007

int add(int a, int b) { return (a + b) % MOD; }
int mul(int a, int b) { return (long long)a * b % MOD; }
pii comb(pii a, pii b) { return {add(mul(a.first, b.second), mul(a.second, b.first)), mul(a.second, b.second)}; }
pii operator+(const pii &l, const pii &r) { return pii(add(l.first, r.first), add(l.second, r.second)); }

void dfs1(int node, bool *vis, vector<int> &comp, vector<pii> *graph)
{
    if (vis[node])
        return;
    vis[node] = 1;
    comp.push_back(node);
    for (auto next : graph[node])
        dfs1(next.first, vis, comp, graph);
}

void dfs2(int node, int par, int ori, vector<pii> *graph, vector<int> &dist, int d = 0)
{
    for (auto next : graph[node])
        if (next.first != par)
        {
            if (ori < next.first)
                dist.push_back(d + next.second);
            dfs2(next.first, node, ori, graph, dist, d + next.second);
        }
}

int main()
{
    // read
    int numMeadows, numRoads, addLen, minLen;
    cin >> numMeadows >> numRoads >> addLen >> minLen;
    int numFarms = numMeadows - numRoads;
    vector<pii> *graph = new vector<pii>[numMeadows];
    for (int i = 0; i < numRoads; i++)
    {
        int n1, n2, dist;
        cin >> n1 >> n2 >> dist;
        n1--, n2--;
        graph[n1].push_back(pii(n2, dist));
        graph[n2].push_back(pii(n1, dist));
    }

    // fill res
    pii *res = new pii[minLen + 1];
    res[min(numFarms * addLen, minLen)] = pii(numFarms * addLen, 1);
    pii *tmp = new pii[minLen + 1];
    bool *vis = new bool[numMeadows];
    fill(vis, vis + numMeadows, false);
    vector<int> comp, dist;
    for (int i = 0; i < numMeadows; i++)
        if (!vis[i])
        {
            comp.clear();
            dist.clear();
            dfs1(i, vis, comp, graph);
            for (auto node : comp)
                dfs2(node, -1, node, graph, dist);
            map<int, pii> m;
            for (auto node : dist)
                m[min(node, minLen)] = m[min(node, minLen)] + pii(node, 1);
            for (int j = 0; j <= minLen; j++)
                tmp[j] = pii(0, 0);
            for (auto node : m)
                for (int j = 0; j <= minLen; j++)
                    tmp[min(j + node.first, minLen)] = tmp[min(j + node.first, minLen)] +
                                                       comb(res[j], node.second);
            for (int j = 0; j <= minLen; j++)
                res[j] = tmp[j];
        }

    // print and delete
    for (int i = 0; i < numFarms - 1; i++)
        tmp[minLen].first = mul(tmp[minLen].first, 2);
    for (int i = 1; i < numFarms; i++)
        tmp[minLen].first = mul(tmp[minLen].first, i);
    cout << tmp[minLen].first << endl;
    delete[] graph;
    delete[] res;
    delete[] tmp;
    delete[] vis;
}