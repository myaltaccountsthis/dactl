/**
 * Author: Ellison Zhu
 * Date: 2026-11-23
 * License: CC0
 * Source: https://cp-algorithms.com/data_structures/treap.html
 * Description: Implicit treap. Handles insert/delete,
 * move/reverse subarrays, and lazy segtree operations.
 * Usage: make_node(x); 
 * Time: O(N\log N)
 * Status: untested
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
mt19937_64 rng(time(0));

struct Node {
    Node *l, *r;
    ll val, sum, add;
    uint64_t w;
    int sz;
    bool rev;
};
const int MAX_NODES = 400'005;
Node pool[MAX_NODES];
int used = 0;

Node* make_node(ll x) {
    Node* t = &pool[used++];
    *t = {0, 0, x, x, 0, rng(), 1, false};
    return t;
}
int size(Node* t) { return t ? t->sz : 0; }
ll sum(Node* t) { return t ? t->sum : 0; }
void pull(Node* t) {
    t->sz = 1 + size(t->l) + size(t->r);
    t->sum = sum(t->l) + t->val + sum(t->r);
}
void add(Node* t, ll x) {
    if (t) t->val += x, t->sum += x * t->sz, t->add += x;
}
void flip(Node* t) {
    if (t) swap(t->l, t->r), t->rev ^= 1;
}
void push(Node* t) {
    if (t->rev) flip(t->l), flip(t->r), t->rev = false;
    if (t->add) add(t->l, t->add), add(t->r, t->add), t->add = 0;
}

Node* merge(Node* a, Node* b) {
    if (!a || !b) return a ? a : b;
    if (a->w > b->w) {
        push(a); a->r = merge(a->r, b);
        pull(a); return a;
    }
    push(b); b->l = merge(a, b->l);
    pull(b); return b;
}
pair<Node*, Node*> split(Node* t, int k) {  // Splits first k elements
    if (!t) return {0, 0};
    push(t);
    Node *a, *b;
    if (k <= size(t->l)) tie(a, t->l) = split(t->l, k), b = t;
    else tie(t->r, b) = split(t->r, k - size(t->l) - 1), a = t;
    pull(t); return {a, b};
}
Node* build(const vector<ll>& v) {
    Node* t = 0;
    for (ll x : v) t = merge(t, make_node(x));
    return t;
}

void update(Node*& t, int l, int r, ll x) {
    Node *a, *b, *c;
    tie(a, b) = split(t, l); tie(b, c) = split(b, r - l);
    add(b, x); t = merge(a, merge(b, c));
}
ll query(Node*& t, int l, int r) {
    Node *a, *b, *c;
    tie(a, b) = split(t, l); tie(b, c) = split(b, r - l);
    ll ans = sum(b);
    t = merge(a, merge(b, c)); return ans;
}
void reverse(Node*& t, int l, int r) {
    Node *a, *b, *c;
    tie(a, b) = split(t, l); tie(b, c) = split(b, r - l);
    flip(b); t = merge(a, merge(b, c));
}
void move(Node*& t, int l, int r, int pos) {
    Node *a, *b, *c;
    tie(a, b) = split(t, l); tie(b, c) = split(b, r - l);
    tie(a, c) = split(merge(a, c), pos);
    t = merge(a, merge(b, c));
}