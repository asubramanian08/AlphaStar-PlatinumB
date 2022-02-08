#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#define NA -1

int removesLeft, szLim;
int check_pathlen(int node, int par, vector<int> *graph)
{
    vector<int> lengths = {0 /*youself*/};
    for (int next : graph[node])
        if (next != par)
            lengths.push_back(check_pathlen(next, node, graph));
    sort(lengths.begin(), lengths.end());
    while (lengths.size() >= 2 && lengths.back() + lengths[lengths.size() - 2] > szLim)
        lengths.pop_back(), removesLeft--;
    return lengths.back() + 1;
}

int main(void)
{
    // read
    int nodes, removes;
    cin >> nodes >> removes;
    vector<int> *graph = new vector<int>[nodes];
    for (int i = 1; i < nodes; i++)
    {
        int n1, n2;
        cin >> n1 >> n2;
        n1--, n2--;
        graph[n1].push_back(n2);
        graph[n2].push_back(n1);
    }

    // binary search pathlength
    int lo = 0, hi = nodes;
    while (hi - lo > 1)
    {
        szLim = (lo + hi) / 2;
        removesLeft = removes;
        check_pathlen(0, NA, graph);
        if (removesLeft < 0)
            lo = szLim;
        else
            hi = szLim;
    }

    // print and delete
    cout << hi << endl;
    delete[] graph;
    return 0;
}