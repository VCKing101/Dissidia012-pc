/* Sends event 3 to every node of the list, then empties it. */
struct Node08883D90 {
    char pad_00[0x6C];
    Node08883D90 *next;
};

struct List08883D90 {
    Node08883D90 *head;
    Node08883D90 *tail;

    static Node08883D90 *after(List08883D90 *l, Node08883D90 *n)
    {
        return n == 0 ? l->head : n->next;
    }
};

extern "C" {

void func_08882F68(Node08883D90 *n, int event);

void func_08883D90(List08883D90 *l)
{
    Node08883D90 *n = l->head;
    while (n != 0) {
        Node08883D90 *next = List08883D90::after(l, n);
        func_08882F68(n, 3);
        n = next;
    }
    l->tail = 0;
    l->head = 0;
}

}
