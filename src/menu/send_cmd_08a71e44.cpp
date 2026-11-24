// Sends command 0x15 with an 8-byte parameter block taken from the global to self (virtual call), then sets bit 4 of the other's flags.
struct Pair { int a; int b; };
struct VBase {
    virtual void v0(); virtual void v1();
    virtual void v2(int other, int cmd, Pair *blk, int len);
};
extern "C" {
extern struct Glob { char g[0xFE8]; Pair p; } D_09B6DF08;
}
extern "C" void func_08A71E44(VBase *self, int other) {
    Pair *src = (Pair *)((char *)&D_09B6DF08 + 0xFE8);
    Pair local = *src;
    self->v2(other, 0x15, &local, 8);
    *(unsigned *)(other + 0x214) = *(unsigned *)(other + 0x214) | 4;
}
