// Destroys every node on the list at +0x3C (virtual deleting dtor), clears the list fields, optionally frees self.
extern "C" void func_08AD07B8(void *self);

struct Node {
    char data[0xF4];
    Node *next;
    virtual void v0(int flags);
};

struct Owner {
    char d0[0x24];
    int f24;
    int f28;
    char d2c[0x10];
    Node *head;
    int f40;
    char d44[0xC4];
    int f108;
    int f10C;
};

extern "C" void func_08AD0170(Owner *o, unsigned int fl) {
    if (o != 0) {
        Node *n = o->head;
        Node *next;
        while (n != 0) {
            next = n == 0 ? o->head : n->next;
            if (n != 0) {
                n->v0(3);
            }
            n = next;
        }
        o->f40 = 0;
        o->head = 0;
        o->f24 = 0;
        o->f28 = 0;
        o->f108 = 0;
        o->f10C = 0;
        if (fl & 1) {
            func_08AD07B8(o);
        }
    }
}
