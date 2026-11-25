// Constructor for a list entry: clears six fields, allocates a 0x48-byte node (func_088954C4 + init func_08895528), stores it at +0 and appends it to the global list at 089D6AD0.
extern "C" {
extern int func_088954C4(int);
extern void func_08895528(void *);
}
struct Node57 {
    int pad0;
    Node57 *next;
};
struct Ent57 {
    Node57 *node;
    float f[6];
};
struct List57 {
    Node57 *head;
    Node57 *tail;
};
extern List57 *D_089D6AD0;
struct Run57 {
    static void append(List57 *l, Node57 *n)
    {
        Node57 **link = (Node57 **)l->tail;
        if (link == 0) {
            l->tail = n;
            l->head = n;
            n->next = 0;
        } else {
            link = link + 1;
            *link = n;
            n->next = 0;
            l->tail = n;
        }
    }
};
extern "C" Ent57 *func_08B113F8(Ent57 *self)
{
    Node57 *n;
    Node57 *made;
    List57 *l;
    self->f[0] = 0.0f;
    self->f[1] = 0.0f;
    self->f[2] = 0.0f;
    self->f[3] = 0.0f;
    self->f[4] = 0.0f;
    self->f[5] = 0.0f;
    n = 0;
    made = (Node57 *)func_088954C4(0x48);
    if (made != 0) {
        func_08895528(made);
        n = made;
    }
    self->node = n;
    l = D_089D6AD0;
    Run57::append(l, n);
    return self;
}
