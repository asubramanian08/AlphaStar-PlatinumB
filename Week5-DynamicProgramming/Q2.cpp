#include <iostream>
#include <vector>
using namespace std;
#define NA -1

pair<int, int> DFS(int node, int par, vector<int> *graph)
{
    pair<int, int> ret(1, 0), curr;
    for (int next : graph[node])
        if (next != par)
        {
            curr = DFS(next, node, graph);
            ret.first += curr.second;
            ret.second += max(curr.first, curr.second);
        }
    return ret;
}

int main(void)
{
    // read
    int nodes;
    cin >> nodes;
    vector<int> *graph = new vector<int>[nodes];
    for (int i = 1, n1, n2; i < nodes; i++)
    {
        cin >> n1 >> n2;
        n1--, n2--;
        graph[n1].push_back(n2);
        graph[n2].push_back(n1);
    }

    // print and delete
    // DFS -> return max cows if this node is used, unused
    pair<int, int> best_vis = DFS(0, NA, graph);
    int ans = max(best_vis.first, best_vis.second);
    cout << ans << endl;
    delete[] graph;
    return 0;
}