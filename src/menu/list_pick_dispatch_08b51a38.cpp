/* Menu list handler: on two accept masks plays sound 2 and goes back; on a third reads the picked entry id and dispatches it, playing sound 1 or 2. */
class Mgr {
public:
    char pad_00[0x34];
    virtual void v0();
    virtual int v1(unsigned long long mask);
};
struct Page { char p[0xC]; Mgr *mgr; char q[8]; int f18; char r[0xA4]; char fC0[4]; };
extern "C" {
extern unsigned long long D_08B52260, D_08B52250, D_08B52258;
void func_088D4BF4(int sound);
int func_088D75F0(void *);
void func_08B51F50(Page *);
void func_08B51D74(Page *, int);
void func_08B51E14(Page *);
void func_08B51DCC(Page *);
void func_08B51A38(Page *self)
{
    if (self->mgr->v1(D_08B52260) != 0 || self->mgr->v1(D_08B52250) != 0) {
        func_088D4BF4(2);
        self->f18 = -1;
        func_08B51F50(self);
    } else if (self->mgr->v1(D_08B52258) != 0) {
        int ok = 1;
        int id = func_088D75F0(&self->fC0);
        self->f18 = id;
        switch (id - 0x24) {
        case 0:
            ok = 0;
            func_08B51F50(self);
            break;
        case 1:
            { int a = 0x29; func_08B51D74(self, a); }
            break;
        case 2:
            { int a = 0x2A; func_08B51D74(self, a); }
            break;
        case 3:
            func_08B51E14(self);
            break;
        case 4:
            func_08B51DCC(self);
            break;
        }
        if (ok) {
            func_088D4BF4(1);
        } else {
            func_088D4BF4(2);
        }
    }
}
}
