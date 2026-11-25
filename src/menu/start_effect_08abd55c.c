/* Starts a timed effect from a computed value, then sets the owner state to 0x20 (starting a sub-job if none runs). */
typedef struct In5C { char pad0[0xA5C]; int state; int job; } In5C;
typedef struct S5C { char pad0[0x13C]; char sub[0x10]; In5C *in; } S5C;
extern int func_088D4C5C(S5C *);
extern void func_08873D34(int, int (*)(void *), float, int, int, int);
extern void func_08873DEC(int, int);
extern void func_088D54DC(void *);
extern int func_08ABF760(void *);
void func_08ABD55C(S5C *self)
{
    float t = (float)func_088D4C5C(self);
    func_08873D34(3, func_08ABF760, t, 0, 1, 2);
    func_08873DEC(3, 2);
    if (self->in->job == 0) {
        func_088D54DC(self->sub);
    }
    self->in->state = 0x20;
}
