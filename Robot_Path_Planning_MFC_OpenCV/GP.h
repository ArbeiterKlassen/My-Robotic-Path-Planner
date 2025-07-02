#pragma once
#include<vector>
#define uint unsigned int
#define INF 114514
namespace Graph_Planner {
	struct Graphans {
		std::vector<std::vector<long>> path;
		std::vector <std::vector<long>> Graph;
		Graphans(std::vector<std::vector<long>> p, std::vector <std::vector<long>> g) {
			path = p;
			Graph = g;
		}
	};
	void Dijkstra(std::vector<std::vector<long>> Graph,int u) {
		std::vector<long> dist;
		std::vector<bool> flag;
		std::vector<long> p;
		uint n = Graph.size();
		flag.resize(n);
		dist.resize(n);
		p.resize(n);
		for (int i = 0; i < n; i++)
		{
			dist[i] = Graph[u][i];
			flag[i] = false;
			if (dist[i] == INF)p[i] = -1;
			else p[i] = u;
		}
		dist[u] = 0;
		flag[u] = true;
		for (int i = 0; i < n; i++)
		{
			int temp = INF;
			int t = u;
			for (int j = 0; j < n; j++)
			{
				if (!flag[j] && dist[j] < temp) {
					t = j;
					temp = dist[j];
				}
			}
			if (t == u)return;
			flag[t] = true;
			for (int j = 0; j < n; j++)
			{
				if(!flag[j] && Graph[t][j] < INF)
					if (dist[j] > dist[t] + Graph[t][j]) {
						dist[j] = dist[t] + Graph[t][j];
						p[j] = t;
					}
			}
		}
	}
	Graphans Floyd(std::vector<std::vector<long>> Graph) {
		int n = Graph.size();
		std::vector<std::vector<long>> p;
		p.resize(n);
		std::vector<std::vector<long>> dist;
		dist.resize(n);
		for (int i = 0; i < n; i++) {
			dist[i].resize(n);
			p[i].resize(n);
			for (int j = 0; j < n; j++)
			{
				dist[i][j] = Graph[i][j];
				if (dist[i][j] < INF && i != j)p[i][j] = i;
				else p[i][j] = -1;
			}
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < n; k++)
				{
					if (dist[i][k] + dist[k][j] < dist[i][j]) {
						dist[i][j] = dist[i][k] + dist[k][j];
						p[i][j] = p[k][j];
					}
				}
			}
		}
		return Graphans(p, dist);
	}
}