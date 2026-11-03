/**
 * Author: Ellison Zhu
 * Date: 2026-11-03
 * License: CC0
 * Source: me
 * Description: Segment tree with range-update range-query capabilities.
 * Use \texttt{lower\_bound()} for segment tree walk.
 * Time: O(\log N).
 * Status: tested on CSES
 */
#pragma once

template <class T, class L>
class LazySegTree {
	int n;
	vector<T> tree;
	vector<L> marked;
public:
	LazySegTree(vector<T>& arr) : n(arr.size()), tree(n * 4), marked(n * 4) {
		build(arr, 1, 0, n - 1);
	}
	void build(vector<T>& arr, int i, int tl, int tr) {
		if (tl == tr)
			tree[i] = arr[tl];
		else {
			int tm = tl + tr >> 1;
			build(arr, i << 1, tl, tm);
			build(arr, i << 1 | 1, tm + 1, tr);
			tree[i] = pull(tree[i << 1], tree[i << 1 | 1]);
		}
	}

	void update(int l, int r, L upd) {
		update(1, 0, n - 1, l, r, upd);
	}
	void update(int i, int tl, int tr, int l, int r, L upd) {
		if (l > r)
			return;
		if (l == tl && tr == r) {
			int sz = tr - tl + 1;
			// IMPL
			// range updates and sums
			// if (upd.second) {
			// 	tree[i] = upd.second * sz;
			// 	marked[i] = upd;
			// }
			// else {
			// 	tree[i] += upd.first * sz;
			// 	marked[i].first += upd.first;
			// }

			// polynomial queries
			// tree[i] += sz * upd.first + tri(sz) * upd.second;
			// marked[i].first += upd.first;
			// marked[i].second += upd.second;
		}
		else {
			int tm = tl + tr >> 1;
			push(i, tm - tl + 1, tr - tm);
			update(i << 1, tl, tm, l, min(r, tm), upd);
			update(i << 1 | 1, tm + 1, tr, max(l, tm + 1), r, upd);
			tree[i] = pull(tree[i << 1], tree[i << 1 | 1]);
		}
	}

	T query(int l, int r) {
		return query(1, 0, n - 1, l, r);
	}
	T query(int i, int tl, int tr, int l, int r) {
		if (l == tl && tr == r)
			return tree[i];
		int tm = tl + tr >> 1;
		push(i, tm - tl + 1, tr - tm);
		if (r <= tm) return query(i << 1, tl, tm, l, r);
		else if (l > tm) return query(i << 1 | 1, tm + 1, tr, l, r);
		else return pull(query(i << 1, tl, tm, l, tm), query(i << 1 | 1, tm + 1, tr, tm + 1, r));
	}

	T pull(T a, T b) {
		// IMPL
		return a + b;
	}
	void push(int i, int sz1, int sz2) {
		// IMPL
		// range updates and sums
		// if (marked[i].second) {
		// 	tree[i << 1] = marked[i].second * sz1;
		// 	tree[i << 1 | 1] = marked[i].second * sz2;
		// 	marked[i << 1] = marked[i << 1 | 1] = {0, marked[i].second};
		// }
		// if (marked[i].first) {
		// 	tree[i << 1] += marked[i].first * sz1;
		// 	tree[i << 1 | 1] += marked[i].first * sz2;
		// 	marked[i << 1].first += marked[i].first;
		// 	marked[i << 1 | 1].first += marked[i].first;
		// }
		// marked[i] = {};
	
		// polynomial queries
		// tree[i << 1] += marked[i] * sz1
		// tree[i << 1 | 1] += marked[i] * sz2
		// marked[i << 1] += marked[i]
		// marked[i << 1 | 1] += marked[i]
	}

	// Segment Tree Walk for O(n log n) binary search
	int lower_bound(T val) {
		// IMPL DEFAULT
		T DEFAULT = 0;
		if (!walkOp(val, tree[1])) return n;
		return walk(1, 0, n - 1, DEFAULT, val);
	}
	bool walkOp(T val, T tval) {
		// IMPL find first value where this is true
		return tval >= val;
	}
	int walk(int i, int tl, int tr, T tval, T val) {
		if (tl == tr)
			return tl;
		int tm = (tl + tr) / 2;
		T comb = pull(tval, tree[i << 1]);
		if (walkOp(val, comb)) return walk(i << 1, tl, tm, tval, val);
		else return walk(i << 1 | 1, tm + 1, tr, comb, val);
	}
};

