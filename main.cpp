#include <bits/stdc++.h>
using namespace std;

/*
We are given multiple queries (x, y) and a preorder traversal of an extended binary tree
where -1 denotes a null node. Nodes have unique values. Two nodes are cousins if they
have the same depth but different parents. For each query, output 1 if they are cousins, else 0.

Approach:
 - Parse all input: first an integer q, then q lines each with two integers x y, then one line of integers
   representing the preorder traversal (including -1 for nulls).
 - Build a mapping value -> (depth, parent_value) by traversing the preorder and constructing the tree recursively.
   We don't need to materialize the tree; we can simulate recursion using an index over the preorder list.
 - For each node value encountered (not -1), record its depth and parent.
 - For each query, look up both nodes. They are cousins if:
     * both exist in the map,
     * depths are equal, and
     * parents are defined and different.

Constraints up to 1000 nodes; recursion depth is safe with iterative stack if needed, but recursion in C++ is fine.
*/

struct Info { int depth; int parent; };

// Recursively process the preorder list and fill info map.
// Returns nothing; advances index as it consumes nodes.
static void build(const vector<int>& pre, int& idx, int depth, int parent,
                  unordered_map<int, Info>& info) {
    if (idx >= (int)pre.size()) return; // safety
    int val = pre[idx++];
    if (val == -1) return; // null node
    info[val] = {depth, parent};
    // left
    build(pre, idx, depth + 1, val, info);
    // right
    build(pre, idx, depth + 1, val, info);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    if (!(cin >> q)) return 0;
    vector<pair<int,int>> queries;
    queries.reserve(max(1, q));
    for (int i = 0; i < q; ++i) {
        int x, y; cin >> x >> y; queries.emplace_back(x, y);
    }

    // Read the rest of the line with preorder traversal.
    vector<int> pre;
    pre.reserve(2000);
    int x;
    while (cin >> x) pre.push_back(x);

    // Build info map from preorder
    unordered_map<int, Info> info;
    info.reserve(pre.size() * 2 + 1);
    int idx = 0;
    build(pre, idx, 0, -1000000007, info); // parent sentinel for root

    // Answer queries
    for (auto [a, b] : queries) {
        auto ia = info.find(a);
        auto ib = info.find(b);
        if (ia == info.end() || ib == info.end()) {
            cout << 0 << '\n';
            continue;
        }
        bool sameDepth = ia->second.depth == ib->second.depth;
        bool diffParent = ia->second.parent != ib->second.parent && ia->second.parent != -1000000007 && ib->second.parent != -1000000007;
        cout << (sameDepth && diffParent ? 1 : 0) << '\n';
    }

    return 0;
}

