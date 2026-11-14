/* Returns the link task of a slot, re-creating it when it links other
   objects than asked for. */
struct LinkTask0883E108 {
    char pad_00[0x24];
    short a;
    char pad_26[0x80 - 0x26];
    int b;
};

struct Slot0883E108 {
    LinkTask0883E108 *task;
};

struct Owner0883E108 {
    int world;
    char pad_04[0x20];
    Slot0883E108 *current;
};

extern "C" {

Slot0883E108 *func_0883DFE0(Owner0883E108 *self, int a, int b, int c);
void func_08808214(LinkTask0883E108 *task);
LinkTask0883E108 *func_08807A00(int size);
void func_0880B8E4(LinkTask0883E108 *task, int world, int c, int a, Slot0883E108 *slot, int b);

LinkTask0883E108 *func_0883E108(Owner0883E108 *self, int a, int b, int c)
{
    Slot0883E108 *s = func_0883DFE0(self, a, b, c);
    if (s->task != 0) {
        if (s->task->a == a && s->task->b == b) {
            self->current = s;
            return s->task;
        }
        func_08808214(s->task);
    }
    LinkTask0883E108 *t = 0;
    LinkTask0883E108 *p = func_08807A00(0x220);
    if (p != 0) {
        func_0880B8E4(p, self->world, c, a, s, b);
        t = p;
    }
    s->task = t;
    self->current = s;
    return s->task;
}

}
