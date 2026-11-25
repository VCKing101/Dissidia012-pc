struct Own0890A080 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7(void *a, void *b, int c);
};
struct Root0890A080 {
    char pad0[0x118];
    unsigned char flag;
    char pad1[0x13E5 - 0x119];
    char tab0[8];
    char pad2[0x139F8 - 0x13ED];
    char dst[1];
    char pad3[0x14C4D - 0x139F9];
    char tab1[8];
    char padx[3];
    unsigned char pick;
    char pad4[0x14C64 - 0x14C59];
    Own0890A080 *owner;
};
extern "C" void func_089C5918(void *);
extern "C" void func_08935524(void *, void *, int);
extern "C" void func_0890A080(Root0890A080 *self, unsigned char *ref);
extern "C" void func_0890A080(Root0890A080 *self, unsigned char *ref)
{
    char out[8];
    unsigned char cur[8];
    if (self->owner != 0) {
        if (self->flag == 1) {
            func_089C5918(cur);
            int i = 0;
            do {
                if (cur[i] < *(unsigned char *)((int)ref + i)) {
                    self->pick = 1;
                    break;
                }
                if (*(unsigned char *)((int)ref + i) < cur[i]) {
                    self->pick = 0;
                    break;
                }
                i = i + 1;
            } while (i < 6);
        }
        if (self->pick == 1) {
            func_08935524(out, self->tab0, 8);
        } else {
            func_08935524(out, self->tab1, 8);
        }
        self->owner->v7(self->dst, out, self->pick);
    }
}
