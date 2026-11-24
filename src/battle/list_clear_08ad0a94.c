/* Walks the list at D_09AFB890 calling func_08AD0170(node, 3) on each, then clears head and tail. */
typedef struct Node { char pad[0x120]; struct Node *next; } Node;
typedef struct ListHead { Node *head; Node *tail; } ListHead;
extern ListHead D_09AFB890;
extern void func_08AD0170(Node *n, int mode);

void func_08AD0A94(void)
{
    Node *n = D_09AFB890.head;
    while (n != 0) {
        Node *next = n == 0 ? D_09AFB890.head : n->next;
        func_08AD0170(n, 3);
        n = next;
    }
    D_09AFB890.tail = 0;
    D_09AFB890.head = 0;
}
