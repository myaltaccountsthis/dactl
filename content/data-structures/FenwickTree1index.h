/**
 * Author: Ellison Zhu
 * Date: 2026-11-03
 * License: CC0
 * Source: me
 * Description: Computes partial sums a[0] + a[1] + ... + a[pos], and updates single elements a[i],
 * taking the difference between the old and new value. 1-indexed, supports \texttt{lower\_bound}.
 * Time: Both operations are $O(\log N)$.
 * Status: Stress-tested, need to check lower\_bound
 */
#pragma once

template <class T>
struct FenwickTree {
	int n;
	vector<T> arr, tree;
	FenwickTree(int n) : n(n), arr(n), tree(n + 1) {}
	void add(int i, T x) {
		arr[i] += x;
		for (i++; i <= n; i += i & -i) tree[i] += x;
	}
	void set(int i, T x) {
		add(i, x - arr[i]);
	}
	T query(int i) {
		T res = 0;
		for (i++; i > 0; i -= i & -i) res += tree[i];
		return res;
	}
	int lower_bound(T x) {
		if (n == 0 || x <= 0) return 0;
		int pos = 0;
		T sum = 0;
		for (int pw = 1 << (31 - __builtin_clz(n)); pw; pw >>= 1) {
			if (pos + pw <= n && sum + tree[pos + pw] < x) {
				pos += pw;
				sum += tree[pos];
			}
		}
		return pos;
	}
};