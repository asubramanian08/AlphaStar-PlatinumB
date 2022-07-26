#include <iostream>
#include <vector>
using namespace std;

// BIT or fenwick tree
class BIT
{
private:
    int *bTree;
    int elements;

public:
    BIT(int elem);
    BIT(int elem, int *initialize, int numInit);
    ~BIT() { delete[] bTree; }
    int query(int node);
    void updt(int node, int cng);
};
BIT::BIT(int elem)
{
    elements = elem;
    bTree = new int[elem + 1];
    for (int i = 1; i <= elem; i++)
        bTree[i] = 0;
}
BIT::BIT(int elem, int *initialize, int numInit)
{
    elements = elem;
    bTree = new int[elem + 1];
    for (int i = 1; i <= elem; i++)
        bTree[i] = 0;
    for (int i = 0; i < numInit; i++)
        updt(i, initialize[i]);
}
int BIT::query(int node)
{
    if (node == -1)
        return 0;
    int sum = 0;
    for (int i = node + 1; i > 0; i -= (i & -i))
        sum += bTree[i];
    return sum;
}
void BIT::updt(int node, int cng)
{
    for (int i = node + 1; i <= elements; i += (i & -i))
        bTree[i] += cng;
}

void DFS(int node, int par, vector<int> *graph, int *highFives, BIT &bit, int *cowAt)
{
    highFives[cowAt[node]] = bit.query(cowAt[node]);
    bit.updt(cowAt[node], 1);
    for (int next : graph[node])
        if (next != par)
            DFS(next, node, graph, highFives, bit, cowAt);
    bit.updt(cowAt[node], -1);
}

int main(void)
{
    // read
    int cowCt, n1, n2;
    cin >> cowCt;
    vector<int> *graph = new vector<int>[cowCt];
    for (int i = 1; i < cowCt; i++)
    {
        cin >> n1 >> n2;
        n1--, n2--;
        graph[n1].push_back(n2);
        graph[n2].push_back(n1);
    }
    int *cowAt = new int[cowCt];
    for (int i = 0; i < cowCt; i++)
    {
        cin >> n1;
        cowAt[n1 - 1] = i;
    }

    // calculate high fives
    int *highFives = new int[cowCt];
    BIT bit(cowCt);
    DFS(0, 0 - 1, graph, highFives, bit, cowAt);

    // print and delete
    for (int i = 0; i < cowCt; i++)
        cout << highFives[i] << endl;
    delete[] graph;
    delete[] cowAt;
    delete[] highFives;
    return 0;
}