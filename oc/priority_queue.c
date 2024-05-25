#define PRIORITY_QUEUE_SIZE 100007

typedef int key_type;
typedef int value_type;

typedef struct {
    key_type key;
    value_type value;
} pqueue_node;

// use array to implement priority queue
pqueue_node pqueue[PRIORITY_QUEUE_SIZE];
int pqueue_node_ctr = 1;

void pqueue_swap(int i, int j) {
    pqueue_node tmp = pqueue[i];
    pqueue[i] = pqueue[j];
    pqueue[j] = tmp;
}

void pqueue_push(key_type key, value_type value) {
    pqueue[pqueue_node_ctr].key = key;
    pqueue[pqueue_node_ctr].value = value;
    int current = pqueue_node_ctr;
    int parent = current / 2;
    while (parent > 0 && pqueue[current].key < pqueue[parent].key) {
        pqueue_swap(current, parent);
        current = parent;
        parent = current / 2;
    }
    pqueue_node_ctr++;
}

void pqueue_pop() {
    pqueue_node_ctr--;
    pqueue[1] = pqueue[pqueue_node_ctr];
    int current = 1;
    int child = current * 2;
    while (child < pqueue_node_ctr) {
        if (child + 1 < pqueue_node_ctr && pqueue[child].key > pqueue[child + 1].key) {
            child++;
        }
        if (pqueue[current].key > pqueue[child].key) {
            pqueue_swap(current, child);
            current = child;
            child = current * 2;
        } else {
            break;
        }
    }
}

pqueue_node pqueue_top() {
    return pqueue[1];
}

int pqueue_empty() {
    return pqueue_node_ctr == 1;
}

int pqueue_size() {
    return pqueue_node_ctr - 1;
}

signed main() {
    return 0;
}
