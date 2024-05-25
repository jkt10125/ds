#define GRAPH_NODE_SIZE 100007

typedef struct {
    int to, next;
} edge_t;

edge_t edge[GRAPH_NODE_SIZE];
int head[GRAPH_NODE_SIZE], edge_cnt;
int size[GRAPH_NODE_SIZE], cat[GRAPH_NODE_SIZE];

int graph_node_size;

void add_edge(int u, int v) {
    edge[edge_cnt].to = v;
    edge[edge_cnt].next = head[u];
    head[u] = edge_cnt++;
    size[u]++;
}

void init_graph(int n) {
    graph_node_size = n;
    for (int i = 0; i < graph_node_size; i++) {
        head[i] = -1;
    }
    edge_cnt = 0;
    ans = 0;
}


void dfs(int x, int p, int cnt) {
    if (cnt > m) {
        return;
    }
    if (size[x] == 1) {
        ans++;
        return;
    }
    for (int i = head[x]; i != -1; i = edge[i].next) {
        int y = edge[i].to;
        if (y == p) {
            continue;
        }
        dfs(y, x, cnt + cat[y]);
    }
}


