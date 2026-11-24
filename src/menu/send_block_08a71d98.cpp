// Sends command 0x14 with a copy of the global 0xEA0-byte block to self (virtual call), then sets bit 2 of the other's flags, clears a field and bit 1 of the state word.
struct Big { int w[0x3A8]; };
struct VBase {
    virtual void v0(); virtual void v1();
    virtual void v2(int other, int cmd, Big *blk, int len);
};
struct Obj : VBase {
    int state;
    int a;
};
extern "C" {
extern struct Glob { char g[0x148]; Big big; } D_09B6DF08;
}
extern "C" void func_08A71D98(Obj *self, int other) {
    Big *src = (Big *)((char *)&D_09B6DF08 + 0x148);
    Big local = *src;
    self->v2(other, 0x14, &local, 0xEA0);
    *(unsigned *)(other + 0x214) = *(unsigned *)(other + 0x214) | 2;
    self->a = 0;
    self->state = self->state & 0xFFFFFFFD;
}
