// Operations on Graph stored as AdjMatrix
#define MAXN 20

struct graph {
	int G[MAXN][MAXN];
	int vnum, arcnum;
};
bool visited[MAXN] = { false };

graph* createGraph(int vnum, int arcnum);
void DFS_Traverse(graph* g, bool visited[]);
void DFS(graph* g, int i);
void BFS_Traverse(graph* g, bool visited[]);
void resetVisited(graph* g, bool visited[]);


graph* createGraph(int vnum, int arcnum) {
	graph* g = new graph;
	g->vnum = vnum;
	g->arcnum = arcnum;
	for (int i = 0; i < vnum; i++) {
		for (int j = 0; j < vnum; j++) {
			g->G[i][j] = 0;
		}
	}
	while (arcnum--) {
		int i, j;
		cin >> i >> j;
		g->G[i][j] = 1;
	}
	return g;
}

void DFS_Traverse(graph* g, bool visited[]) {
	for (int i = 0; i < g->vnum; i++) {
		if (!visited[i]) {
			DFS(g, i);
		}
	}
}

void DFS(graph* g, int i) {
	visited[i] = true;
	cout << i << " ";
	for (int j = 0; j < g->vnum; j++) {
		if (g->G[i][j]  && (!visited[j])) {
			DFS(g, j);
		}
	}
}

void BFS_Traverse(graph* g, bool visited[]) {
	queue<int> q;
	for (int i = 0; i < g->vnum; i++) {
		if (!visited[i]) {
			visited[i] = true;
			cout << i << " ";
			q.push(i);
			while (!q.empty()){
				q.pop();
				for (int j = 0; j < g->vnum; j++) {
					if (g->G[i][j] && (!visited[j])) {
						visited[j] = true;
						cout << j << " ";
						q.push(j);
					}
				}
			}
		}
	}
}

void resetVisited(graph* g, bool visited[]) {
	for (int i = 0; i < g->vnum; i++) {
		visited[i] = false;
	}
}