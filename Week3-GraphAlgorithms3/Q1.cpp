#include <iostream>
#include <vector>
#include <array>
using namespace std;

// Dynamic least common ancestor forest
#define NA -1
#define GLOB_LOG 30
class DynamicLCAF // 1 indexed!
{
private:
    vector<array<int, GLOB_LOG + 1>> sparseTBL;
    vector<int> dpt;
    vector<vector<int>> tree;

public:
    DynamicLCAF();
    int query(int node1, int node2);
    int depth(int node) { return dpt[node]; }
    void add(int par);
};
DynamicLCAF::DynamicLCAF()
{
    sparseTBL.push_back(array<int, GLOB_LOG + 1>());
    sparseTBL[0].fill(NA);
    dpt.push_back(-1); // to support forest
    tree.push_back(vector<int>());
}
int DynamicLCAF::query(int node1, int node2)
{
    // fix nodes
    if (dpt[node1] < dpt[node2])
        swap(node1, node2);
    for (int i = GLOB_LOG; i >= 0; i--) // loop though bits
        if (dpt[node1] - (1 << i) >= dpt[node2])
            node1 = sparseTBL[node1][i];
    if (node1 == node2) // n2 is ancestor
        return node2 == 0 ? NA : node2;

    // main compute
    for (int i = GLOB_LOG; i >= 0; i--)
        if (sparseTBL[node1][i] != sparseTBL[node2][i])
            node1 = sparseTBL[node1][i],
            node2 = sparseTBL[node2][i];
    return sparseTBL[node1][0] == 0 ? NA : sparseTBL[node1][0];
}
void DynamicLCAF::add(int par)
{
    int node = dpt.size();
    if (par == -2 || par == NA)
        par = 0;
    sparseTBL.push_back(array<int, GLOB_LOG + 1>());
    sparseTBL.back().fill(NA);
    sparseTBL[node][0] = par;
    for (int i = 1; (i <= GLOB_LOG) && (sparseTBL[node][i - 1] != NA); i++)
        sparseTBL[node][i] = sparseTBL[sparseTBL[node][i - 1]][i - 1];
    dpt.push_back(dpt[par] + 1);
    tree[par].push_back(node);
    tree.push_back({});
}

int main(void)
{
    // setup
    int queries, node;
    char action;
    cin >> queries;
    vector<int> treeNum = {0};
    DynamicLCAF dlf; // Dynamic LCA forest
    auto dist = [&dlf](int n1, int n2)
    { return dlf.depth(n1) + dlf.depth(n2) -
             2 * dlf.depth(dlf.query(n1, n2)); };
    using pii = pair<int, int>;
    vector<pii> diameter;

    // process
    for (int q = 0; q < queries; q++)
    {
        cin >> action >> node;
        if (action == 'Q')
        {
            cout << max(dist(node, diameter[treeNum[node]].first),
                        dist(node, diameter[treeNum[node]].second))
                 << endl;
            continue;
        }
        // action == 'B'
        dlf.add(node);
        if (node == NA)
        {
            diameter.push_back(pii(treeNum.size(), treeNum.size()));
            treeNum[node = 0] = diameter.size() - 1;
        }
        treeNum.push_back(treeNum[node]);
        if (dist(treeNum.size() - 1, diameter[treeNum.back()].first) >
            dist(diameter[treeNum.back()].first, diameter[treeNum.back()].second))
            diameter[treeNum.back()].second = treeNum.size() - 1;
        if (dist(treeNum.size() - 1, diameter[treeNum.back()].second) >
            dist(diameter[treeNum.back()].first, diameter[treeNum.back()].second))
            diameter[treeNum.back()].first = treeNum.size() - 1;
    }
    return 0;
}