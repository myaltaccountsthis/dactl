/**
 * Author: Ellison Zhu, Quang Nhat Ngo
 * Date: 2026-11-06
 * Source: https://sotanishy.github.io/cp-library-cpp/data-structure/segtree/persistent_segment_tree.hpp
 * Description: Array-based persistent segment tree
 * Usage: arrays[k] = arrays[k].update(i, x); // update
 * arrays[k].query(l, r);   // query
 * arrays.push_back(arrays[k]); // make copy
 * Status: tested
*/
#pragma once

template <typename T>
class PersistentSegmentTree {
public:
    PersistentSegmentTree() = default;
    explicit PersistentSegmentTree(int n)
        : PersistentSegmentTree(vector<T>(n, id())) {}
    explicit PersistentSegmentTree(const vector<T>& v)
        : nodes(make_shared<vector<Node>>(1)), root(0), size(bit_ceil(v.size())) {
		nodes->reserve(size);
        build(v, root, 0, size);
    }

    PersistentSegmentTree update(int k, const T& x) const {
        return PersistentSegmentTree(update(k, x, root, 0, size), size, nodes);
    }

    T query(int l, int r) const { return query(l, r, root, 0, size); }

private:
    static T id() {    // identity element
        return 0;
    }
    static T pull(T a, T b) {
        return a + b;
    }

    struct Node {
        T val;
        int left, right;
        Node() : val(id()), left(-1), right(-1) {}
        Node(const T& val, int left, int right)
            : val(val), left(left), right(right) {}
    };

    shared_ptr<vector<Node>> nodes; // Copies share the pool, not individual nodes.
    int root;
    int size;

    PersistentSegmentTree(int root, int size, const shared_ptr<vector<Node>>& nodes)
        : nodes(nodes), root(root), size(size) {}

    int new_node(const Node& n = Node()) const {
        nodes->push_back(n);
        return (int)nodes->size() - 1;
    }

    void build(const vector<T>& v, int n, int l, int r) const {
        auto& tree = *nodes;
        if (r - l == 1) {
            tree[n].val = l < (int)v.size() ? v[l] : id();
            return;
        }
        int m = midpoint(l, r);
        tree[n].left = new_node();
        build(v, tree[n].left, l, m);
        tree[n].right = new_node();
        build(v, tree[n].right, m, r);
        tree[n].val = pull(tree[tree[n].left].val, tree[tree[n].right].val);
    }

    int update(int k, const T& x, int n, int l, int r) const {
        auto& tree = *nodes;
        if (r - l == 1) {
            return new_node(Node(x, -1, -1));
        }
        int m = midpoint(l, r);
        if (k < m) {
            auto left = update(k, x, tree[n].left, l, m);
            T val = pull(tree[left].val, tree[tree[n].right].val);
            return new_node(Node(val, left, tree[n].right));
        } else {
            auto right = update(k, x, tree[n].right, m, r);
            T val = pull(tree[tree[n].left].val, tree[right].val);
            return new_node(Node(val, tree[n].left, right));
        }
    }

    T query(int a, int b, int n, int l, int r) const {  // [l, r)
        auto& tree = *nodes;
        if (r <= a || b <= l) return id();
        if (a <= l && r <= b) return tree[n].val;
        int m = midpoint(l, r);
        return pull(query(a, b, tree[n].left, l, m), query(a, b, tree[n].right, m, r));
    }
};
