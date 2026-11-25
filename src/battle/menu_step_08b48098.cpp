// Menu step handler: polls a list, on selection change swaps entry highlights, else key masks set mode 2 or play a sound.
struct Mgr08B48098 {
    char pad[0x34];
    virtual int v0(unsigned long long k);
    virtual int v1(unsigned long long k);
};
struct Menu08B48098 {
    char pad0[0xBC];
    char entries[0x4C];
    char pad1[0x4E8 - 0xBC - 0x4C];
    Mgr08B48098 *mgr;
    int list;
    int sel;
    char pad2[0x518 - 0x4F4];
    int mode;
    int other;
};
extern "C" {
extern unsigned long long D_08B48510;
extern unsigned long long D_08B48518;
void func_088D688C(void *, int);
void func_088D4BF4(int);
void func_08B47E48(void *);
void func_08B4758C(void *);
void func_08B4753C(void *);
void func_08B48098(Menu08B48098 *self)
{
    int old;
    old = self->sel;
    self->mode = 0;
    func_088D688C(&self->list, 0);
    if (old != self->sel) {
        func_08B4758C((char *)self + old * 0x4C + 0xBC);
        func_08B4753C((char *)self + self->sel * 0x4C + 0xBC);
        self->mode = 1;
    } else {
        if (self->mgr->v1(D_08B48510)) {
            self->mode = 2;
        } else if (self->mgr->v1(D_08B48518)) {
            if (self->other != self->sel) {
                func_088D4BF4(1);
                func_08B47E48(self);
            } else {
                func_088D4BF4(4);
                return;
            }
        }
    }
}
}
