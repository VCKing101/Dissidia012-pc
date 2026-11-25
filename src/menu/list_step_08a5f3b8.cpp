/* Menu list step: plays sound 2 on the first mask, sound 1 on the second, otherwise redraws when either list cursor moved. */
class Mgr {
public:
    char pad_00[0x34];
    virtual void v0();
    virtual int v1(unsigned long long mask);
};
struct Page {
    char p0[0x2C]; int f2C;
    char p1[0x320 - 0x30]; Mgr *mgr; int f324; int f328;
    char p2[0x350 - 0x32C]; int f350; int f354;
};
extern "C" {
extern unsigned long long D_08A601A0, D_08A601A8;
void func_088D4BF4(int sound);
int func_088D688C(void *, int);
void func_08A5F26C(Page *);
void func_08A5F1A4(Page *);
void func_08A5F830(Page *);
void func_08A5F3B8(Page *self)
{
    if (self->mgr->v1(D_08A601A0) != 0) {
        func_088D4BF4(2);
        func_08A5F26C(self);
        self->f2C = 0;
    } else if (self->mgr->v1(D_08A601A8) != 0) {
        int one = 1;
        func_088D4BF4(1);
        func_08A5F1A4(self);
        self->f2C = one;
    } else {
        int a = self->f354;
        int b = self->f328;
        func_088D688C(&self->f324, 0);
        func_088D688C(&self->f350, 0);
        if (b != self->f328 || a != self->f354) {
            func_08A5F830(self);
        }
    }
}
}
