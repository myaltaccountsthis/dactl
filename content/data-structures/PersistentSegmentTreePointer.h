/**
 * Author: Quang Nhat Ngo, Ellison Zhu
 * Date: 2026-11-06
 * Source: https://sotanishy.github.io/cp-library-cpp/data-structure/segtree/persistent_segment_tree.hpp
 * Description: Generic persistent segment tree
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
        : root(make_shared<Node>()), size(bit_ceil(v.size())) {
        build(v, root, 0, size);
    }

    PersistentSegmentTree update(int k, const T& x) const {
        return PersistentSegmentTree(update(k, x, root, 0, size), size);
    }

    T query(int l, int r) const { return query(l, r, root, 0, size); }

private:
    static T id() {    // identity element
        return 0;
    }
    static T pull(T a, T b) {
        return a + b;
    }
    
    struct Node;
    using node_ptr = shared_ptr<Node>;
    struct Node {
        T val;
        node_ptr left, right;
        Node() : val(id()), left(nullptr), right(nullptr) {}
        Node(const T& val, const node_ptr& left, const node_ptr& right)
            : val(val), left(left), right(right) {}
    };

    node_ptr root;
    int size;

    PersistentSegmentTree(const node_ptr& root, int size)
        : root(root), size(size) {}

    void build(const vector<T>& v, const node_ptr& n, int l, int r) const {
        if (r - l == 1) {
            n->val = l < (int)v.size() ? v[l] : id();
            return;
        }
        int m = midpoint(l, r);
        n->left = make_shared<Node>();
        build(v, n->left, l, m);
        n->right = make_shared<Node>();
        build(v, n->right, m, r);
        n->val = pull(n->left->val, n->right->val);
    }

    node_ptr update(int k, const T& x, const node_ptr& n, int l, int r) const {
        if (r - l == 1) {
            return make_shared<Node>(x, nullptr, nullptr);
        }
        int m = midpoint(l, r);
        if (k < m) {
            auto left = update(k, x, n->left, l, m);
            T val = pull(left->val, n->right->val);
            return make_shared<Node>(val, left, n->right);
        } else {
            auto right = update(k, x, n->right, m, r);
            T val = pull(n->left->val, right->val);
            return make_shared<Node>(val, n->left, right);
        }
    }

    T query(int a, int b, const node_ptr& n, int l, int r) const {  // [l, r)
        if (r <= a || b <= l) return id();
        if (a <= l && r <= b) return n->val;
        int m = midpoint(l, r);
        return pull(query(a, b, n->left, l, m), query(a, b, n->right, m, r));
    }
};