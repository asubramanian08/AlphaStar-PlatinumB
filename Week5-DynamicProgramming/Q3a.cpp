#include <iostream>
#include <vector>
#include <set>
using namespace std;
#define NA -1

pair<int, bool> bsearchAns(const multiset<int> &children, int limit, int complete)
{
    vector<int> childs;
    for (auto child : children)
        childs.push_back(child);
    for (int i = 0; i < complete; i++)
        childs.push_back(limit);
    if (childs.size() % 2 == 0)
        childs.insert(childs.begin(), 0);
    int lo = NA, hi = childs.size(), mid, l, r;
    bool works;
    while (hi - lo > 1)
    {
        works = true;
        mid = (hi + lo) / 2, l = 0, r = childs.size() - 1;
        for (int i = 0; i < childs.size() / 2 && works; i++)
        {
            l += l == mid;
            r -= r == mid;
            if (childs[l] + childs[r] < limit)
                works = false;
            l++, r--;
        }
        (works ? lo : hi) = mid;
    }
    int val1 = lo == NA ? NA : childs[lo] + 1;
    works = true;
    l = 1, r = childs.size() - 1;
    if ((children.size() + complete) % 2)
    {
        childs.insert(childs.begin(), 0);
        l = 0;
    }
    for (int i = 0; i < childs.size() / 2 && works; i++)
    {
        if (childs[l] + childs[r] < limit)
            works = false;
        l++, r--;
    }
    return make_pair(val1, works);
}

int DFS(int node, int par, vector<int> *graph, int limit)
{
    multiset<int> children;
    int complete = 0;
    for (int next : graph[node])
        if (next != par)
            if (int curr = DFS(next, node, graph, limit); curr == NA)
                return NA;
            else if (curr >= limit)
                complete++;
            else
                children.insert(curr);
    pair<int, bool> ret = bsearchAns(children, limit, complete);
    if (node == 0)
        return ret.second;
    if (ret.first != NA)
        return ret.first;
    if (ret.second)
        return 1;
    return NA;
}

int main(void)
{
    int nodes;
    cin >> nodes;
    vector<int> *graph = new vector<int>[nodes];
    for (int i = 1, n1, n2; i < nodes; i++)
    {
        cin >> n1 >> n2;
        n1--, n2--;
        graph[n1].push_back(n2);
        graph[n2].push_back(n1);
    };
    int lo = 1, hi = nodes, md;
    while (hi - lo > 1)
        if (DFS(0, NA, graph, md = (lo + hi) / 2) > 0)
            lo = md;
        else
            hi = md;
    cout << lo << endl;
    delete[] graph;
    return 0;
}