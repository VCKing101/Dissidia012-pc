/* Menu list step: runs the shared update, then on the first mask flags the entry and moves to state 3, on the second clears the flag, otherwise forwards the list query. */
class Mgr {
public:
    char pad_00[0x34];
    virtual void v0();
    virtual int v1(unsigned long long mask);
};
struct Sub { char p0[4]; int state; char p1[0x534]; char list[0x2C]; char f568; };
struct Page { char p[0x584]; Sub *sub; };
extern "C" {
extern unsigned long long D_08AB2FF0, D_08AB2FF8;
Mgr *func_088729A0(int index);
void func_088D4BF4(int sound);
int func_088D688C(void *, int);
void func_08AB2A30(Page *);
void func_08AB2D24(Page *);
int func_08AB2874(Page *self)
{
    func_08AB2D24(self);
    if (func_088729A0(0)->v1(D_08AB2FF0) != 0) {
        func_088D4BF4(1);
        { Sub *u = self->sub; int one = 1; u->f568 = one; }
        { Sub *u = self->sub; int three = 3; u->state = three; }
    } else if (func_088729A0(0)->v1(D_08AB2FF8) != 0) {
        func_088D4BF4(2);
        self->sub->f568 = 0;
        { Sub *u = self->sub; int three = 3; u->state = three; }
    } else if (func_088D688C(self->sub->list, 0) != 0) {
        func_08AB2A30(self);
    }
    return 0;
}
}
