/* Spawns an effect, registers it with the global manager and appends its node to the owner's node list. */
extern "C" {
typedef struct Node { char pad0[0x10]; unsigned flags; char pad1[0x4C - 0x14]; struct Node *next; int pad2; struct Node *prev; } Node;
typedef struct Owner { char pad0[0x30]; Node *head; Node *tail; int count; } Owner;
typedef struct Attach { int handle; Owner *owner; Node *node; } Attach;
extern Node *func_0887B3EC(int, int, int, int, int, int);
extern int func_088D0700(void *, unsigned char, int, int);
extern void func_088CF970(void *, int);
extern Owner *func_088DAA68(int);
extern void *D_089D7830;
struct Loader {
    static int Load(unsigned char kind, int arg) { return func_088D0700(D_089D7830, kind, arg, -1); }
};
void func_08AF3BA4(Attach *self, int src, int kind, int arg) {
    int h;
    Node *n;
    Owner *o;
    self->node = func_0887B3EC(src, 2, 0, 0, 0, 0);
    h = Loader::Load((unsigned char)kind, arg);
    self->handle = h;
    func_088CF970(D_089D7830, h);
    self->owner = func_088DAA68(self->handle);
    o = self->owner;
    n = self->node;
    if (n != 0) {
        int c;
        n->flags |= 0x8000;
        Node *t = o->tail;
        if (t == 0) {
            o->tail = n;
            o->head = n;
            n->next = 0;
            n->prev = 0;
            c = o->count;
        } else {
            Node **lk = &t->next;
            *lk = n;
            t = o->tail;
            n->next = 0;
            n->prev = t;
            c = o->count;
            o->tail = n;
        }
        o->count = c + 1;
    }
}
}
