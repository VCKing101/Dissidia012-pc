/* Deletes every voice of the list (flags 3), then empties it. */
struct Node088A5EBC {
    char pad_00[0x40];
    Node088A5EBC *next;
};

struct List088A5EBC {
    Node088A5EBC *head;
    Node088A5EBC *tail;

    static Node088A5EBC *after(List088A5EBC *l, Node088A5EBC *n)
    {
        return n == 0 ? l->head : n->next;
    }
};

extern "C" {

void func_088A43EC(Node088A5EBC *n, unsigned int flags);

void func_088A5EBC(List088A5EBC *l)
{
    Node088A5EBC *n = l->head;
    while (n != 0) {
        Node088A5EBC *next = List088A5EBC::after(l, n);
        func_088A43EC(n, 3);
        n = next;
    }
    l->tail = 0;
    l->head = 0;
}

}
