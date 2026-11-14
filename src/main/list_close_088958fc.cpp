/* Sends event 3 to every node of the list, then empties it. */
struct Node088958FC {
    int field_00;
    Node088958FC *next;
    virtual void handle(int event);
};

struct List088958FC {
    Node088958FC *head;
    Node088958FC *tail;
    int count;

    static Node088958FC *after(List088958FC *l, Node088958FC *n)
    {
        return n == 0 ? l->head : n->next;
    }
};

extern "C" {

void func_088958FC(List088958FC *l)
{
    Node088958FC *n = l->head;
    while (n != 0) {
        Node088958FC *next = List088958FC::after(l, n);
        if (n != 0) {
            n->handle(3);
        }
        n = next;
    }
    l->tail = 0;
    l->head = 0;
    l->count = 0;
}

}
