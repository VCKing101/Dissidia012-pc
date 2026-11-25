struct Node {
    int pad0[4];
    unsigned flags;
    int pad1[(0x4C - 0x14) / 4];
    Node *next;
    int pad2;
    Node *prev;
};

struct List {
    int pad[0xC];
    Node *head;
    Node *tail;
    int count;
};

struct Owner {
    int pad0[2];
    List *list;
    char sub[0x30];
    char done;
};

extern "C" Node *func_0887A4C4(void *sub);
extern "C" int func_0887A484(void *sub);
extern "C" int func_0887A48C(void *sub);
extern "C" void func_0887A354(void *sub);

struct Ops {
    static void unlink(Node *n, List *l) {
        if (n != 0) {
            n->flags = n->flags & 0xFFFF7FFF;
            if (n->prev == 0) {
                l->head = n->next;
            } else {
                Node **pn = &n->prev->next;
                *pn = n->next;
            }
            Node **pp;
            if (n->next == 0) {
                l->tail = n->prev;
            } else {
                pp = &n->next->prev;
                *pp = n->prev;
            }
            n->prev = 0;
            n->next = 0;
            l->count = l->count - 1;
        }
    }
};

extern "C" void func_088CD538(Owner *self) {
    char *sub = (char *)self + 0xC;
    Node *n = func_0887A4C4(sub);
    if (self->list != 0 && n != 0) Ops::unlink(n, self->list);
    {
        int r = func_0887A484(sub);
        if (r == 1) {
            r = func_0887A48C(sub);
            if (r == 0) {
                int one2 = 1;
                self->done = one2;
            }
        }
    }
    func_0887A354(sub);
}
