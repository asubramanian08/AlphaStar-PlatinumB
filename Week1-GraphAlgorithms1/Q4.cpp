// Barely out of time on test case 2
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
using namespace std;
using pii = pair<int, int>;
#define NA -1
#define mp make_pair
#define BIG_NUM 1000000000

int main(void)
{
    // read
    int numFields, pathways, grassType, updates;
    cin >> numFields >> pathways >> grassType >> updates;
    numFields++;
    vector<pii> *graph = new vector<pii>[numFields];
    for (int i = 0; i < pathways; i++)
    {
        int n1, n2, wt;
        cin >> n1 >> n2 >> wt;
        graph[n1].push_back(mp(wt, n2));
        graph[n2].push_back(mp(wt, n1));
    }
    int *color = new int[numFields];
    for (int i = 1; i < numFields; i++)
        cin >> color[i];
    color[0] = NA;

    // minimum spanning tree
    vector<pii> *mst = new vector<pii>[numFields];
    pii *parent = new pii[numFields];
    for (int i = 0; i < numFields; i++)
        parent[i] = mp(NA, NA);
    priority_queue<pair<pii, int>, vector<pair<pii, int> >, greater<pair<pii, int> > > pq;
    pq.push(mp(mp(0, 1), 0));
    while (!pq.empty())
    {
        pii conn = pq.top().first;
        int par = pq.top().second;
        pq.pop();
        if (parent[conn.second].first != NA)
            continue;
        parent[conn.second] = mp(par, conn.first);
        mst[par].push_back(conn);
        for (pii next : graph[conn.second])
            pq.push(mp(next, conn.second));
    }

    // start up heaps
    map<int, set<pii> > *localHeap = new map<int, set<pii> >[numFields];
    for (int i = 0; i < numFields; i++)
        for (pii next : mst[i])
            localHeap[i][color[next.second]].insert(next);
    auto smltLocal = [&localHeap](int fld, int col)
    {
        auto start = localHeap[fld][col].begin();
        if (start == localHeap[fld][col].end())
            return BIG_NUM;
        return start->first;
    };
    set<pair<int, pii> > globalHeap;
    for (int i = 0; i < numFields; i++)
        for (const auto &heap : localHeap[i])
            if (color[i] != heap.first)
                globalHeap.insert(mp(heap.second.begin()->first, mp(i, heap.first)));
    auto updtGlob = [&globalHeap](int currWt, int prevWt, int par, int col)
    {
        globalHeap.erase(mp(prevWt, mp(par, col)));
        if (currWt != BIG_NUM)
            globalHeap.insert(mp(currWt, mp(par, col)));
    };

    // update and answer
    int field, grass, orgColor, prevVal, temp;
    for (int updt = 0; updt < updates; updt++)
    {
        cin >> field >> grass;
        orgColor = color[field];
        color[field] = grass;
        prevVal = smltLocal(parent[field].first, orgColor);
        localHeap[parent[field].first][orgColor].erase(mp(parent[field].second, field));
        if (color[parent[field].first] != orgColor)
            updtGlob(smltLocal(parent[field].first, orgColor), prevVal, parent[field].first, orgColor);
        prevVal = smltLocal(parent[field].first, grass);
        localHeap[parent[field].first][grass].insert(mp(parent[field].second, field));
        if (color[parent[field].first] != grass)
            updtGlob(smltLocal(parent[field].first, grass), prevVal, parent[field].first, grass);
        globalHeap.erase(mp(smltLocal(field, grass), mp(field, grass)));
        if (temp = smltLocal(field, orgColor); temp != BIG_NUM)
            globalHeap.insert(mp(temp, mp(field, orgColor)));
        while (!globalHeap.empty() && globalHeap.begin()->second.first == 0)
            globalHeap.erase(globalHeap.begin());
        cout << globalHeap.begin()->first << endl;
    }

    // delete
    delete[] graph;
    delete[] color;
    delete[] mst;
    delete[] parent;
    delete[] localHeap;
    return 0;
}