// Operations on Graph stored as AdjMatrix
#define MAXN 20
#define INF 0x3fffffff

struct graph {
	int G[MAXN][MAXN];
	int vnum, arcnum;
};

bool visited[MAXN];
int dist[MAXN];
int path[MAXN];

void initArray(graph* g);
graph* createGraph(int vnum, int arcnum);
void DFS_Traverse(graph* g, bool visited[]);
void DFS(graph* g, int i);
void BFS_Traverse(graph* g, bool visited[]);
void resetVisited(graph* g, bool visited[]);
int findMinDist(graph* g, bool visited[], int dist[], int i);
void Dijkstra(graph* g, bool visited[], int dist[], int path[]);
void print(graph* g, int data[]);


void initArray(graph* g) {
	for (int i = 0; i < g->vnum; i++) {
		visited[i] = false;
		dist[i] = INF;
		path[i] = -1;
	}
}

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
		if (g->G[i][j] && (!visited[j])) {
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
			while (!q.empty()) {
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

int findMinDist(graph* g, bool visited[], int dist[], int i) {
	int minIndex = i;
	for (int j = i; j < g->vnum; j++) {
		if ((!visited[j]) && dist[j] < dist[minIndex]) {
			minIndex = j;
		}
	}
	return minIndex;
}

void Dijkstra(graph* g, bool visited[], int dist[], int path[]) {
	dist[0] = 0;
	for (int i = 0; i < g->vnum; i++) {
		int v = findMinDist(g, visited, dist, i);
		if (v == -1) break;
		visited[v] = true;
		for (int w = 0; w < g->vnum; w++) {
			if (g->G[v][w] && (!visited[w])) {
				if (dist[v] + g->G[v][w] < dist[w]) {
					dist[w] = dist[v] + g->G[v][w];
					path[w] = v;
				}
			}
		}
	}
}

void print(graph* g, int data[]) {
	for (int i = 0; i < g->vnum; i++) {
		cout << data[i] << " ";
	}
	cout << endl;
}