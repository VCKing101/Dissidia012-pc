/* Menu list step: handles the three accept masks with sounds, otherwise redraws when the list cursor moved. */
class Mgr {
public:
    char pad_00[0x34];
    virtual void v0();
    virtual int v1(unsigned long long mask);
};
struct Page {
    char p0[0x10EC]; Mgr *mgr; int f10F0; int f10F4;
};
extern "C" {
extern unsigned long long D_08A58688, D_08A58690, D_08A58698;
void func_088D4BF4(int sound);
int func_088D688C(void *, int);
int func_08A580B0(Page *);
int func_08A58068(Page *);
void func_08A57F28(Page *);
void func_08A57A18(Page *self)
{
    if (self->mgr != 0) {
        if (self->mgr->v1(D_08A58688) != 0) {
            if (func_08A580B0(self) != 0) {
                func_088D4BF4(2);
            }
            return;
        }
        if (self->mgr->v1(D_08A58690) != 0) {
            if (func_08A58068(self) != 0) {
                func_088D4BF4(1);
            }
            return;
        }
        if (self->mgr->v1(D_08A58698) != 0) {
            func_088D4BF4(1);
            self->f10F4 = 6;
            func_08A57F28(self);
            return;
        }
    }
    {
        int old = self->f10F4;
        func_088D688C(&self->f10F0, 0);
        if (old != self->f10F4) {
            func_08A57F28(self);
        }
    }
}
}
