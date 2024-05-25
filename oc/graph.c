#define GRAPH_NODE_SIZE 100007
#define GRAPH_EDGE_SIZE 100007

typedef struct {
    int to, next, weight;
} weighted_edge;

weighted_edge weighted_edge_list[GRAPH_EDGE_SIZE];
int edge_list_head[GRAPH_NODE_SIZE], graph_edge_ctr;

int graph_node_size;

void graph_init(int n) {
    graph_node_size = n;
    graph_edge_ctr = 0;
    for (int i = 0; i < graph_node_size; i++) {
        edge_list_head[i] = -1;
    }
}
void add_weighted_edge(int from, int to, int weight) {
    weighted_edge_list[graph_edge_ctr].to = to;
    weighted_edge_list[graph_edge_ctr].weight = weight;
    weighted_edge_list[graph_edge_ctr].next = edge_list_head[from];
    edge_list_head[from] = graph_edge_ctr++;
}

void dfs(int x, int p) {
    for (int z = edge_list_head[x], y; z != -1; z = weighted_edge_list[z].next) {
        y = weighted_edge_list[z].to;
        w = weighted_edge_list[z].weight;
        // The edge between x and y has weight w.
        if (y != p) {
            dfs(y, x);
            // ...
        }
    }
}

void bfs(int x) {
    // c does not have queue functions.
    int q[graph_node_size], qh = 0, qt = 0;
    q[qt++] = x;
    while (qh < qt) {
        x = q[qh++];
        for (int z = edge_list_head[x], y; z != -1; z = weighted_edge_list[z].next) {
            y = weighted_edge_list[z].to;
            w = weighted_edge_list[z].weight;
            // The edge between x and y has weight w.
            // ...
            q[qt++] = y;
        }
    }
}
