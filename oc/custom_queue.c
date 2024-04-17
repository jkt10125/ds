#define QUEUE_SIZE 100007

typedef int queue_type;

struct queue {
    queue_type (*queue_function)(queue_type, queue_type);
    queue_type push_stack[QUEUE_SIZE][2], pop_stack[QUEUE_SIZE][2];
    int push_stack_size, pop_stack_size;
} Qu;

void queue_init(queue_type (*f)(queue_type, queue_type)) {
    Qu.queue_function = f;
    Qu.push_stack_size = Qu.pop_stack_size = 0;
}

void queue_transfer_impl() {
    if (Qu.pop_stack_size == 0) {
        for (int i = 0; i < Qu.push_stack_size; ++i) {
            Qu.pop_stack[i][0] = Qu.push_stack[Qu.push_stack_size - i - 1][0];
            Qu.pop_stack[i][1] = Qu.queue_function(Qu.pop_stack[i][0], i ? Qu.pop_stack[i - 1][1] : Qu.pop_stack[i][0]);
        }
        Qu.pop_stack_size = Qu.push_stack_size;
        Qu.push_stack_size = 0;
    }
}

void queue_push(queue_type val) {
    Qu.push_stack[Qu.push_stack_size][0] = val;
    Qu.push_stack[Qu.push_stack_size][1] = Qu.queue_function(val, Qu.push_stack_size ? Qu.push_stack[Qu.push_stack_size - 1][1] : val);
    Qu.push_stack_size++;
}

void queue_pop() {
    queue_transfer_impl();
    Qu.pop_stack_size--;
}

int queue_size() {
    return Qu.push_stack_size + Qu.pop_stack_size;
}

queue_type queue_front() {
    queue_transfer_impl();
    return Qu.pop_stack[Qu.pop_stack_size - 1][0];
}

queue_type queue_function_output() {
    queue_transfer_impl();
    return Qu.push_stack_size ? Qu.queue_function(Qu.push_stack[Qu.push_stack_size - 1][1], Qu.pop_stack[Qu.pop_stack_size - 1][1]) : Qu.pop_stack[Qu.pop_stack_size - 1][1];
}

int f(int a, int b) {
    return a < b ? a : b;
}

int main() {
    queue_init(f); // f should be idempotent
    return 0;
}