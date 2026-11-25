/* Opens the dialog: registers or re-uses the handle, clears the owner's pending state, stores the previous selection and starts a timed effect. */
typedef struct In86B90 { char pad0[0x910]; int cur; int prev; int clr; char pad1[0x968 - 0x91C]; unsigned char flag; } In86B90;
typedef struct S86B90 { char pad[0x14C]; In86B90 *in; } S86B90;
extern int D_089D69E8;
extern int func_08885544(int);
extern void func_088850EC(int, int, int (*)(void *));
extern void func_08885340(int, int);
extern void func_088852C4(int, int, int);
extern void func_088854C4(int, int, int);
extern void func_08885388(int);
extern void func_08A8799C(S86B90 *);
extern int func_088D4C5C(void);
extern void func_08873D34(int, int (*)(void *), float, int, int, int);
extern void func_08873DB0(float *, int);
extern int func_08A87E00(void *);
extern int func_08A87490(void *);
void func_08A86B90(S86B90 *self)
{
    float p[4];
    float t;
    float zero;
    if (!func_08885544(D_089D69E8)) {
        func_088850EC(D_089D69E8, 0x22, func_08A87E00);
        { int zi = 0; func_08885340(D_089D69E8, zi); }
    } else {
        { int on = 1; int zi = 0; func_088852C4(D_089D69E8, on, zi); }
    }
    self->in->clr = 0;
    func_088854C4(D_089D69E8, 0x22, 0x405);
    func_08885388(D_089D69E8);
    func_08A8799C(self);
    self->in->flag = 0;
    self->in->cur = self->in->prev;
    t = (float)func_088D4C5C();
    func_08873D34(2, func_08A87490, t, 0, 1, 2);
    zero = 0.0f;
    p[0] = zero; p[1] = zero; p[2] = zero; p[3] = zero;
    func_08873DB0(p, 2);
}
