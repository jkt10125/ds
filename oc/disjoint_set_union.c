#define DSU_SIZE 100007

int dsu_node[DSU_SIZE];

void dsu_init() {
    for (int i = 0; i < DSU_SIZE; ++i) {
        dsu_node[i] = -1;
    }
}

int dsu_find(int x) {
    return dsu_node[x] < 0 ? x : (dsu_node[x] = dsu_find(dsu_node[x]));
}

int dsu_merge(int x, int y) {
    if ((x = dsu_find(x)) == (y = dsu_find(y))) {
        return 0;
    }
    if (dsu_node[x] > dsu_node[y]) {
        swap(x, y);
    }
    dsu_node[x] += dsu_node[y];
    dsu_node[y] = x;
    return 1;
}

int dsu_size(int x) {
    return -dsu_node[dsu_find(x)];
}

int main() {
    dsu_init();
    return 0;
}