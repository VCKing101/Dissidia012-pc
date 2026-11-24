/* Returns 1 if any node in the first non-empty list head (slots at +0x1B8) has flag 2 set. */
typedef struct Node { char pad[2]; unsigned short flags; char pad2[0x10]; struct Node *next; } Node;
int func_08941268(int a, int *obj)
{
    Node *n = 0;
    int i;
    for (i = 0; i < 2; i++) {
        Node *h = (Node *)obj[0x1B8 / 4 + i];
        if (h) { n = h; break; }
    }
    while (n) {
        if (n->flags & 2) return 1;
        n = n->next;
    }
    return 0;
}
