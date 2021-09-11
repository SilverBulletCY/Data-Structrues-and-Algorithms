// Operations on Graph stored as AdjList
#define MAXN 20
#define INF 0x3fffffff

struct node {
	int data;
	node* next;
};

struct graph {
	node* v[MAXN];
	int vnum, arcnum;
};

bool visited[MAXN];
int dist[MAXN];
int path[MAXN];

void initArray(graph* g);
graph* createGraph(int vnum, int arcnum);
void DFS_Traverse(graph* g, bool visited[]);
void DFS(graph* g, int i);
void resetVisited(graph* g);
void BFS_Traverse(graph* g, bool visited[]);
int findMinIndex(graph* g, bool visited[], int dist[], int i);
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
	for (int i = 0; i < g->vnum; i++) {
		node* temp = new node;
		temp->data = i;
		temp->next = NULL;
		g->v[i] = temp;
	}
	while (arcnum--) {
		int i, j;
		cin >> i >> j;
		node* temp = new node;
		temp->data = j;
		temp->next = NULL;
		node* p = g->v[i];
		while (p->next) p = p->next;
		p->next = temp;
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
	node* q = g->v[i];
	while (q->next) {
		q = q->next;
		if (!visited[q->data]) {
			DFS(g, q->data);
		}
	}
}

void resetVisited(graph* g) {
	for (int i = 0; i < g->vnum; i++) {
		visited[i] = false;
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
				node* s = g->v[i];
				while (s->next) {
					s = s->next;
					if (!visited[s->data]) {
						visited[s->data] = true;
						cout << s->data << " ";
						q.push(s->data);
					}
				}
			}
		}
	}
}

int findMinIndex(graph* g, bool visited[], int dist[], int i) {
	int minIndex = i, flag = 0;
	for (int j = i; j < g->vnum; j++) {
		if ((!visited[j]) && dist[j] < dist[minIndex]) {
			minIndex = j;
			flag = 1;
		}
	}
	return minIndex;
}

void Dijkstra(graph* g, bool visited[], int dist[], int path[]) {
	dist[0] = 0;
	for (int i = 0; i < g->vnum; i++) {
		int t = findMinIndex(g, visited, dist, i);
		visited[t] = true;
		node* s = g->v[t];
		while (s->next) {
			s = s->next;
			if (!visited[s->data]) {
				if (dist[t] + 1 < dist[s->data]) {
					dist[s->data] = dist[t] + 1;
					path[s->data] = t;
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