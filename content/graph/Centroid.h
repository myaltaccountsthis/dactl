/**
 * Author: Ellison Zhu
 * Date: 2026-11-03
 * License: CC0
 * Source: https://cp-algorithms.com/graph/centroid_decomposition.html
 * Description: Decomposes a tree into centroids. Handles queries
 * along paths, such as counting paths with K edges. Divide and conquer
 * approach lets it compute paths within a centroid, then between centroids.
 * E is int if unweighted, pair if weighted. 
 * Usage: CentroidDecomp<int> cd(adjList); cd.decompose(0); cd.ans
 * Time: O(N\log N)
 * Status: tested on CSES Fixed Length Paths
 */
#pragma once

using ll = long long;

template <class E>
struct CentroidDecomp {
	int n, k;
	vector<vector<E>> adjList;
	vector<bool> removed;
	vector<int> subsz;
	ll ans = 0;

	CentroidDecomp(const vector<vector<E>>& adjList, int k) : n(adjList.size()), k(k), adjList(adjList), removed(n, false), subsz(n) {}

	int getSize(int node, int parent = -1) {
		subsz[node] = 1;
		for (int adj : adjList[node]) {
			if (removed[adj] || adj == parent)
				continue;
			subsz[node] += getSize(adj, node);
		}
		return subsz[node];
	}

	int getCentroid(int node, int tsz, int parent = -1) {
		for (int adj : adjList[node]) {
			if (removed[adj] || adj == parent)
				continue;
			if (subsz[adj] * 2 > tsz)
				return getCentroid(adj, tsz, node);
		}
		return node;
	}

	void getBest(int node, int parent, E dist, vector<E>& distances) {
		if (dist > k)	// k2 if [k1, k2]
			return;
		distances.push_back(dist);
		for (int adj : adjList[node]) {
			if (removed[adj] || adj == parent)
				continue;
			// getBest(adj, node, make_pair(dist.first + 1, dist.second + weight), distances);	// weighted
			getBest(adj, node, dist + 1, distances);	// normal
		}
	}

	void processCentroid(int centroid, int size) {
		vector<ll> allDistances(size);	// normal
		allDistances[0] = 1;
		// FenwickTree fenwick(size); // fixed length paths 2
		// fenwick.add(0, 1);
		// vector<ll> best(size, -1e18);	// weighted
		// best[0] = 0;

		for (int adj : adjList[centroid]) {	// normal
			if (removed[adj])
				continue;
			
			vector<E> distances;
			distances.reserve(subsz[adj]);
			getBest(adj, centroid, 1, distances); // normal
			// getBest(adj, centroid, make_pair(1, weight), distances); // weighted
			
			// IMPL merge step - first find paths with other centroids
			// then update best/count among centroids already seen
			// normal
			for (int dist : distances) {
				if (k - dist >= 0 && k - dist < size)
					ans += allDistances[k - dist];
			}
			for (int dist : distances) {
				allDistances[dist]++;
			}
			// maximum weight for path length k
			// for (auto [dist, total] : distances) {
			// 	if (k - dist >= 0 && k - dist < size)
			// 		ans = max(ans, total + best[k - dist]);
			// }
			// for (auto [dist, total] : distances) {
			// 	best[dist] = max(best[dist], total);
			// }
			// fixed length paths 2
			// for (int dist : distances) {
			// 	ans += fenwick.query(min(size - 1, k2 - dist)) - fenwick.query(min(size - 1, k1 - dist) - 1);
			// }
			// for (int dist : distances) {
			// 	fenwick.add(dist, 1);
			// }
		}
	}

	void decompose(int node) {
		int size = getSize(node);
		int centroid = getCentroid(node, size);
		processCentroid(centroid, size);

		removed[centroid] = true;
		for (int adj : adjList[centroid]) {	// normal
			if (removed[adj])
				continue;
			decompose(adj);
		}
	}
};