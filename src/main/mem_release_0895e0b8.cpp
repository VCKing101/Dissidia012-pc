// Frees the block through the manager of the current owner and subtracts its size from that owner's usage counter.
struct C { char pad[0x18]; unsigned short slot; };
struct A { char pad[0x1C8]; C *c; };
struct O { char pad[0x18]; int mgr; char pad2[0x6C]; A *a; char pad3[0x28]; int cnt[8]; };
extern "C" O *volatile D_09B6F478;
extern "C" int func_08971010(int, int);
struct R {
    static void run(int block) {
        O *o;
        o = D_09B6F478;

        int n = func_08971010(o->mgr, block);
        O *q = (O *)((char *)o + o->a->c->slot * 4);
        q->cnt[0] = q->cnt[0] - n;
    }
};
extern "C" void func_0895E0B8(int block) { R::run(block); }
