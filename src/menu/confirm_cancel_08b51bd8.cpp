/* Menu confirm/cancel handler: on the first mask plays sound 2 and acts, on the second plays sound 1 and acts on the result of the list query. */
class Mgr {
public:
    char pad_00[0x34];
    virtual void v0();
    virtual int v1(unsigned long long mask);
};
struct Page { char p[0xC]; Mgr *mgr; char q[0x0C]; int f1C; };
extern "C" {
extern unsigned long long D_08B52250, D_08B52258;
void func_088D4BF4(int sound);
int func_088DE7E8(void *);
void func_08B51E64(Page *);
void func_08B51F50(Page *);
void func_08B51BD8(Page *self)
{
    if (self->mgr->v1(D_08B52250) != 0) {
        func_088D4BF4(2);
        func_08B51E64(self);
    } else if (self->mgr->v1(D_08B52258) != 0) {
        func_088D4BF4(1);
        switch (func_088DE7E8(&self->f1C)) {
        case 0:
            func_08B51F50(self);
            break;
        case 1:
            func_08B51E64(self);
            break;
        }
    }
}
}
