/* Sends event 3 to every node of the list, then empties it. */
struct Node08883DF0 {
    char pad_00[0x70];
    Node08883DF0 *next;
};

struct List08883DF0 {
    Node08883DF0 *head;
    Node08883DF0 *tail;

    static Node08883DF0 *after(List08883DF0 *l, Node08883DF0 *n)
    {
        return n == 0 ? l->head : n->next;
    }
};

extern "C" {

void func_08882F68(Node08883DF0 *n, int event);

void func_08883DF0(List08883DF0 *l)
{
    Node08883DF0 *n = l->head;
    while (n != 0) {
        Node08883DF0 *next = List08883DF0::after(l, n);
        func_08882F68(n, 3);
        n = next;
    }
    l->tail = 0;
    l->head = 0;
}

}
