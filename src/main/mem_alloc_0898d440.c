// Allocates the block (size at least 1) through the manager of the current owner and adds the granted size to that owner's usage counter.
struct C { char pad[0x18]; unsigned short slot; };
struct A { char pad[0x1C8]; struct C *c; };
struct O { char pad[0x18]; int mgr; char pad2[0x6C]; struct A *a; char pad3[0x28]; int cnt[8]; };
extern struct O *D_09B6F478;
extern int func_08971134(int, int, int *, int);
void func_0898D440(int size) {
    struct { int got; struct O *o; } l;
    int ok;
    struct O *w;
    if (size == 0) {
        size = 1;
    }
    struct O *o0 = D_09B6F478;
    int m = o0->mgr;
    l.o = o0;
    ok = func_08971134(m, size, &l.got, 1);
    w = l.o;
    if (ok != 0) {
        struct O *q = (struct O *)((char *)w + w->a->c->slot * 4);
        q->cnt[0] = q->cnt[0] + l.got;
    }
}
