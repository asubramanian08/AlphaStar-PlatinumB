// Works 1/15 -> don't know why path matching does work
#include <iostream>
#include <vector>
#include <set>
using namespace std;
#define NA -1

int DFS(int node, int par, vector<int> *graph, int limit)
{
    // find lower path length
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
    // eleminate matching paths (add to >= limit)
    for (multiset<int>::iterator first = children.begin(), rem;
         children.size(); first = children.begin())
    {
        int pathSz = *first;
        children.erase(first);
        if (rem = children.lower_bound(limit - pathSz); rem != children.end())
            children.erase(rem);
        else
        {
            children.insert(pathSz);
            break;
        }
    }
    // return the correct val (the connecting path len)
    if (node == 0)
        return complete - children.size();
    if (children.size() < complete)
        return limit;
    if (children.size() == complete && children.size() == 0)
        return 1;
    if (multiset<int>::iterator pos; children.size() - 1 >= complete)
        return *--(pos = children.end()) + 1;
    return NA; // too many children
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