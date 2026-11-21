/* Idle-state update: in state 2 picks one of two follow-ups from a random bit, otherwise refreshes the unit and ends the state. */
typedef struct Obj38 { char pad[0x158]; void *h158; void *h15c; void *h160; } Obj38;
typedef struct Ref38 { int pad; struct Unit38 *unit; int pad2[10]; int state; } Ref38;
typedef struct Unit38 { char pad[0x12A9]; unsigned char flag; } Unit38;
extern Ref38 *func_08AB3264(void *h);
extern int func_08AA5748(int p);
extern void func_08A69254(void *h);
extern void func_08A69298(void *h);
extern void func_08A9B928(Obj38 *self);
extern void *func_08AB327C(void *h);
extern int func_08AAEC0C(void *u);
extern int func_08AB249C(void *u);
extern void func_08A9BDAC(Obj38 *self, int a);
extern void func_08A9BEAC(Obj38 *self, int a, int b);
extern void func_08A97698(void *r);
extern void func_08A9B8A8(Obj38 *self);
extern int D_08AA5828;
int func_08A9F8A0(Obj38 *self, int arg)
{
    void *u;
    int v;
    int r;
    Ref38 *ref;
    if (arg == 0) {
        return -1;
    } else {
        if (func_08AB3264(self->h160)->state == 2) {
            r = func_08AA5748(D_08AA5828);
            if (r % 2 == 0) {
                func_08A69254(self->h15c);
            } else {
                func_08A69298(self->h15c);
            }
        } else {
            func_08A9B928(self);
            u = func_08AB327C(self->h160);
            v = func_08AAEC0C(u);
            if (func_08AB249C(u) == 0) {
                func_08A9BDAC(self, 1);
            } else {
                func_08A9BEAC(self, 1, v);
                ref = func_08AB3264(self->h160);
                if (ref->unit == 0) {
                    return -2;
                }
                if (ref->unit->flag == 1) {
                    func_08A97698(func_08AB3264(self->h160));
                }
            }
            func_08A9B8A8(self);
        }
        return 0;
    }
}
