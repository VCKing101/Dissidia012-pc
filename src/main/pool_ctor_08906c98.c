/* Initialises a 16-slot pool owned by a manager and links its list node into the manager. */
typedef struct Node { struct Node *next; } Node;
typedef struct Owner { char pad[0x194]; Node *head; Node *tail; } Owner;
typedef struct Slot {
    char pad0[8];
    int cells[10];
    char pad1[0x58 - 0x30];
    int x, y;
    char z;
    char pad2[0x80 - 0x61];
} Slot;
typedef struct Pool {
    Owner *owner;
    Slot slots[16];
    char pad[0x10CC - 0x804];
    Node node;
    char pad2[0x1648 - 0x10D0];
    int a, b, c, d;
    char pad3[0x16D8 - 0x1658];
    int e, f;
    char pad4[5];
    char g;
} Pool;
extern void func_08905848(void *);
void func_08906C98(Pool *self, Owner *owner, int a, int b, int c, int d)
{
    int i;
    Slot *s;
    Owner *o;
    Node *t;
    self->owner = owner;
    o = owner;
    t = o->tail;
    if (t == 0) {
        Node *n = &self->node;
        o->tail = n;
        o->head = n;
        self->node.next = 0;
    } else {
        Node *n = &self->node;
        t->next = n;
        self->node.next = 0;
        o->tail = n;
    }
    self->a = a; self->b = b; self->c = c; self->d = d;
    self->e = 0; self->f = 0;
    self->g = 0;
    for (i = 0, s = (Slot *)((char *)self + 4); i < 16; i++, s++) {
        int j;
        int *p, *q;
        func_08905848(s);
        s->x = 0;
        s->y = 0;
        s->z = 0;
        for (j = 0, p = &s->cells[0], q = &s->cells[10]; j < 5; j++, p += 2, q += 2) {
            p[0] = 0; p[1] = 0; q[0] = 0; q[1] = 0;
        }
    }
}
