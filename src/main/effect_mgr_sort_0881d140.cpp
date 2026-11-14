/* Effect manager: sorts every node, and the actors owning them, into the
   five per-frame update lists. */
struct Actor0881D140 {
    char pad_000[0x5FC];
    Actor0881D140 *next_c;
    Actor0881D140 *next_d;
    Actor0881D140 *next_e;
};

struct Node0881D140 {
    char pad_00[4];
    Node0881D140 *next_a;
    Node0881D140 *next_b;
    char pad_0C[0x14 - 0xC];
    Actor0881D140 *owner;
};

template <class T, T *T::*Next> struct List0881D140 {
    T *head;
    T *tail;

    static T *&next_of(T *p) { return p->*Next; }

    void append(T *x)
    {
        if (this->tail == 0) {
            this->tail = x;
            this->head = x;
            next_of(x) = 0;
        } else {
            next_of(this->tail) = x;
            next_of(x) = 0;
            this->tail = x;
        }
    }
};

struct EffectMgr0881D140 {
    char pad_000[0x118];
    List0881D140<Node0881D140, &Node0881D140::next_a> list_a;
    List0881D140<Node0881D140, &Node0881D140::next_b> list_b;
    List0881D140<Actor0881D140, &Actor0881D140::next_c> list_c;
    List0881D140<Actor0881D140, &Actor0881D140::next_d> list_d;
    List0881D140<Actor0881D140, &Actor0881D140::next_e> list_e;
};

extern "C" {

Node0881D140 *func_0881CB80(Node0881D140 *node);
int func_0881D624(EffectMgr0881D140 *self, Node0881D140 *node);
int func_0881D6AC(EffectMgr0881D140 *self, Node0881D140 *node);
int func_0881D518(EffectMgr0881D140 *self, Node0881D140 *node);
int func_0881D55C(EffectMgr0881D140 *self, Node0881D140 *node);
int func_0881D5A0(EffectMgr0881D140 *self, Node0881D140 *node);

int func_0881D140(EffectMgr0881D140 *self)
{
    Node0881D140 *n = 0;
    while ((n = func_0881CB80(n)) != 0) {
        Actor0881D140 *owner = n->owner;
        if (func_0881D624(self, n) == 1) {
            self->list_a.append(n);
        }
        if (func_0881D6AC(self, n) == 1) {
            self->list_b.append(n);
        }
        if (func_0881D518(self, n) == 1) {
            self->list_c.append(owner);
        }
        if (func_0881D55C(self, n) == 1) {
            self->list_d.append(owner);
        }
        if (func_0881D5A0(self, n) == 1) {
            self->list_e.append(owner);
        }
    }
    return 0;
}

}
