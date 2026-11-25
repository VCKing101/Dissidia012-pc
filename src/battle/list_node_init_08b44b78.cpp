// Initialises a list node and appends it to the owner's list.
extern char D_08B45BC0[];
struct Node;
struct List { Node *head; Node *tail; };
struct Owner { char pad[8]; List list; };
struct Node {
    Owner *owner; int value; int zero; Node *next; unsigned char flag; Node *vt;
    static Node *run(Node *n, Owner *owner, int value)
    {
        List *l;
        Node **q;
        n->vt = (Node *)D_08B45BC0;
        n->owner = owner;
        n->value = 0;
        l = &owner->list;
        q = (Node **)l->tail;
        if (q == 0) {
            l->tail = n;
            l->head = n;
            n->next = 0;
        } else {
            q = q + 3;
            *q = n;
            n->next = 0;
            l->tail = n;
        }
        n->value = value;
        n->zero = 0;
        n->flag = 0;
        return n;
    }
};
extern "C" Node *func_08B44B78(Node *n, Owner *owner, int value) { return Node::run(n, owner, value); }
