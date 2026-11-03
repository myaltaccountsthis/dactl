/**
 * Author: Simon Lindholm, Ellison Zhu
 * Date: 2026-11-03
 * License: CC0
 * Source: folklore
 * Description: Computes sums a[i,j] for all $i\le I, j\le J$, and increases single elements a[i,j].
 *  Do coordinate compression first if needed. Remove grid if no set() calls.
 * Time: $O(\log^2 N)$. (Use persistent segment trees for $O(\log N)$.)
 * Status: not tested
 */
#pragma once

#include "FenwickTree1index.h"

template <class T>
struct FenwickTree2 {
    int n, m;
	vector<vector<T>> grid; vector<FenwickTree<T>> ft;
	FenwickTree2(int n, int m) : n(n), m(m), grid(n, vector<T>(m)), ft(n + 1, m) {}
	void add(int i, int j, T x) {
        grid[i][j] += x;
		for (i++; i <= n; i += i & -i)
			ft[i].add(j, x);
	}
    void set(int i, int j, T x) {
        add(i, j, x - grid[i][j]);
    }
	T query(int i, int j) {
		T sum = 0;
		for (i++; i > 0; i -= i & -i)
			sum += ft[i].query(j);
		return sum;
	}
};
