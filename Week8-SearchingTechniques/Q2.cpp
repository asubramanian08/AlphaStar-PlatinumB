#include <iostream>
using namespace std;

// Segment Tree: longest prefix, suffix, and interval of 0s
class LazySegTree
{
private:
    using pii = pair<int, int>;
    pii *cover;
    enum propagate { NA, add, remove };
    propagate *lazy;
    int *prefix, *suffix, *interval;
    int coverSize(int pos) { return cover[pos].second - cover[pos].first + 1; }
    bool canInsert(int seqLen, int pos) { return interval[pos] >= seqLen; }
    void updateParent(int pos)
    {
        prefix[pos] = prefix[2 * pos + 0];
        if (prefix[pos] == coverSize(2 * pos + 0))
            prefix[pos] += prefix[2 * pos + 1];
        suffix[pos] = suffix[2 * pos + 1];
        if (suffix[pos] == coverSize(2 * pos + 1))
            suffix[pos] += suffix[2 * pos + 0];
        interval[pos] = max(max(interval[2 * pos + 0], interval[2 * pos + 1]),
                            suffix[2 * pos + 0] + prefix[2 * pos + 1]);
    }
    void updateLazy(int pos)
    {
        if (lazy[pos] == NA) return;
        prefix[pos] = suffix[pos] = interval[pos] =
            lazy[pos] == add ? 0 : coverSize(pos);
        if (coverSize(pos) > 1)
            lazy[2 * pos + 0] = lazy[2 * pos + 1] = lazy[pos];
        lazy[pos] = NA;
    }
    void initTree(int pos, int left, int right)
    {
        cover[pos] = pii(left, right), lazy[pos] = NA;
        if (left == right) { prefix[pos] = suffix[pos] = interval[pos] = 1; return; }
        initTree(2 * pos + 0, left, (left + right) / 2 + 0);
        initTree(2 * pos + 1, (left + right) / 2 + 1, right);
        updateParent(pos);
    }

public:
    LazySegTree(int size)
    {
        cover = new pii[size * 3];
        lazy = new propagate[size * 3];
        prefix = new int[size * 3];
        suffix = new int[size * 3];
        interval = new int[size * 3];
        initTree(1, 0, size - 1);
    }
    ~LazySegTree()
    {
        delete[] cover;
        delete[] lazy;
        delete[] prefix;
        delete[] suffix;
        delete[] interval;
    }
    bool seat(int seqLen, int pos = 1)
    {
        updateLazy(pos);
        if (interval[pos] < seqLen) return false;
        if (coverSize(pos) == 1)
        {
            update(cover[pos].first, cover[pos].second, true);
            return true;
        }
        if (seat(seqLen, 2 * pos + 0) || seat(seqLen, 2 * pos + 1)) return true;
        int start = cover[2 * pos + 0].second + 1 - suffix[2 * pos + 0];
        update(start, start + seqLen, true);
        return true;
    }
    void update(int left, int right, bool toAdd, int pos = 1)
    {
        // fully covered
        if (left <= cover[pos].first && cover[pos].second <= right)
            lazy[pos] = toAdd ? add : remove;
        updateLazy(pos);
        // no intersection
        if (cover[pos].second < left || right < cover[pos].first) return;
        // some intersection
        if (coverSize(pos) > 1 && (cover[pos].first < left || right < cover[pos].second))
        {
            update(left, right, toAdd, 2 * pos + 0);
            update(left, right, toAdd, 2 * pos + 1);
            updateParent(pos);
        }
    }
    bool treeTest(int pos)
    {
        if (coverSize(pos) == 1)
            return prefix[pos] == suffix[pos] && suffix[pos] == interval[pos];
        updateLazy(pos);
        if(lazy[pos * 2 + 0] != NA || lazy[pos * 2  + 1]!=NA)
            return true;
        int p = prefix[2 * pos + 0];
        if (p == coverSize(2 * pos + 0))
            p += prefix[2 * pos + 1];
        int s = suffix[2 * pos + 1];
        if (s == coverSize(2 * pos + 1))
            s += suffix[2 * pos + 0];
        int i = max(max(interval[2 * pos + 0], interval[2 * pos + 1]),
                            suffix[2 * pos + 0] + prefix[2 * pos + 1]);
        return (p == prefix[pos]) && (s == suffix[pos]) && (i == interval[pos])
            && treeTest(2*pos+0) && treeTest(2*pos+1);
    }
};

int main(void)
{
    int cowCt, queries, turnedAway = 0, num1, num2; char type;
    cin >> cowCt >> queries;
    LazySegTree seg(cowCt);
    for (int i = 0; i < queries; i++)
    {
        cin >> type >> num1;
        if(!seg.treeTest(1))
            printf("HELP");
        if (type == 'A' && !seg.seat(num1)) turnedAway++;
        else if (type == 'L') { cin >> num2; seg.update(num1 - 1, num2 - 1, false); }
    }
    cout << turnedAway << endl;
    return 0;
}