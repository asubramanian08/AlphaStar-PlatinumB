#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using pii = pair<int, int>;
#define NA -1

int heightDFS(int node, int par, vector<pii> *forest, int *height)
{
    height[node] = 0;
    for (const pii &next : forest[node])
        if (next.first != par)
            height[node] = max(height[node], heightDFS(next.first, node, forest, height) + next.second);
    return height[node];
}
int SL_DFS(int node, int par, vector<pii> *forest, int *height, int parDist)
{
    vector<int> branchLens = {parDist};
    for (const pii &next : forest[node])
        if (next.first != par)
            branchLens.push_back(height[next.first] + next.second);
    sort(branchLens.begin(), branchLens.end(), greater<int>());
    int SLDist = branchLens.front();
    for (const pii &next : forest[node])
        if (next.first != par)
            SLDist = min(SLDist, SL_DFS(next.first, node, forest, height,
                                        branchLens[height[next.first] + next.second == branchLens[0]] + next.second));
    return SLDist;
}

int main(void)
{
    freopen("hi.txt", "r", stdin);
    // read
    int nodeCt, connections, newTravelTime;
    cin >> nodeCt >> connections >> newTravelTime;
    vector<pii> *forest = new vector<pii>[nodeCt];
    for (int i = 0; i < connections; i++)
    {
        int n1, n2, wt;
        cin >> n1 >> n2 >> wt;
        forest[n1].push_back(pii(n2, wt));
        forest[n2].push_back(pii(n1, wt));
    }

    // shortest longest distences for each tree
    int *height = new int[nodeCt];
    for (int i = 0; i < nodeCt; i++)
        height[i] = NA;
    vector<int> SLDists;
    for (int i = 0; i < nodeCt; i++)
        if (height[i] == NA)
        {
            heightDFS(i, NA, forest, height);
            SLDists.push_back(SL_DFS(i, NA, forest, height, 0));
            for (int i = 0; i < nodeCt; i++)
                cout << height[i] << endl;
        }

    // print and delete
    sort(SLDists.begin(), SLDists.end(), greater<int>());
    cout << (SLDists[0] + (SLDists.size() >= 2 ? SLDists[1] + newTravelTime : 0)) << endl;
    delete[] forest;
    delete[] height;
}