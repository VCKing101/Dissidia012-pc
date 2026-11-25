/* Menu confirm/cancel handler returning 1 on the first mask, 2 or 3 from the list query result on the second, 0 otherwise. */
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
void func_08B51970(Page *);
int func_08B51874(Page *self)
{
    int res = 0;
    if (self->mgr->v1(D_08B52250) != 0) {
        func_088D4BF4(2);
        func_08B51970(self);
        res = 1;
    } else if (self->mgr->v1(D_08B52258) != 0) {
        func_088D4BF4(1);
        func_08B51970(self);
        switch (func_088DE7E8(&self->f1C)) {
        case 0:
            res = 3;
            break;
        case 1:
            res = 2;
            break;
        }
    }
    return res;
}
}
