/* If the effect slot is free: sets a state bit, starts a timed effect and clears a four-float parameter block. */
typedef struct S39C20 { char pad[0x44]; unsigned flags; } S39C20;
extern int func_08873EB4(S39C20 *);
extern void func_08873D34(int, int (*)(void *), float, int, int, int);
extern void func_08873DB0(float *, int);
extern int func_08B3CB08(void *);
void func_08B39C20(S39C20 *self)
{
    if (func_08873EB4(self)) {
        float p[4];
        float zero;
        float t = 15.0f;
        self->flags |= 2;
        func_08873D34(4, func_08B3CB08, t, 0, 1, 2);
        zero = 0.0f;
        p[0] = zero; p[1] = zero; p[2] = zero; p[3] = zero;
        func_08873DB0(p, 2);
    }
}
