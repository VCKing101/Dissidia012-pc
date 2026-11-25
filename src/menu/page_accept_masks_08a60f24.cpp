/* Menu page step: checks two accept masks; on a hit starts the part and stores the result index in the state. */
class Mgr {
public:
    char pad_00[0x34];
    virtual void v0();
    virtual int v1(unsigned long long mask);
};
struct Page { char p0[0x2C]; int state; char p1[0xF4]; Mgr *mgr; };
extern "C" {
extern unsigned long long D_08A61DE0, D_08A61DE8;
void func_08A60944(Page *);
void func_088D4BF4(int);
void func_08A60F24(Page *self)
{
    self->state = 0;
    if (self->mgr->v1(D_08A61DE0) != 0) {
        func_08A60944(self);
        func_088D4BF4(2);
        self->state = 2;
    } else if (self->mgr->v1(D_08A61DE8) != 0) {
        func_08A60944(self);
        func_088D4BF4(1);
        self->state = 1;
        return;
    }
}
}
