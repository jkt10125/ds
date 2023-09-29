#include <bits/stdc++.h>

const int N = 100007;

class MDS {
    struct node {
        int val;
        node *left, *right;
    } *head, *tail;
    std::vector<node *> HASH[N];
    size_t size;

    public:
    MDS() : head(nullptr), tail(nullptr), size(0) {}

    void insert_back(int val) {
        node *new_node = new node;
        new_node->val = val;
        if (!size) { head = tail = new_node; }
        new_node->left = tail;
        new_node->right = head;
        tail->right = new_node;
        head->left = new_node;
        tail = new_node;
        HASH[val].push_back(new_node);
        size++;
    }

    bool erase(int val) {
        if (HASH[val].empty()) return false;
        node *cur = HASH[val].back();
        HASH[val].pop_back();
        if (cur == head) head = cur->right;
        if (cur == tail) tail = cur->left;
        if (cur->left) cur->left->right = cur->right;
        if (cur->right) cur->right->left = cur->left;
        delete cur; size--;
        return true;
    }

    void move_to_back(int val) {
        if (erase(val)) {
            insert_back(val);
        }
    }

    std::vector<int> iterate() {
        std::vector<int> res;
        int i = size;
        for (node *cur = head; i --> 0; cur = cur->right) {
            res.push_back(cur->val);
        }
        return res;
    }
};

int main() {

    MDS mds;
    mds.insert_back(1);
    mds.insert_back(2);

    for (int x : mds.iterate()) {
        std::cout << x << ' ';
    }
}