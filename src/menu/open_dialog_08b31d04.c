/* Opens the dialog: registers (or re-uses) the handle with its callback, clears the owner's cursor, builds the page, flags it open and starts a timed effect. */
typedef struct In31D04 { char page[0x400]; int open; int cur; } In31D04;
typedef struct S31D04 { char pad[0x138]; In31D04 *in; } S31D04;
extern int D_089D69E8;
extern int func_08885544(int);
extern void func_088850EC(int, int, int (*)(void *));
extern void func_08885340(int, int);
extern void func_088852C4(int, int, int);
extern void func_088854C4(int, int, int);
extern void func_08885388(int);
extern void func_088D3910(int, void *, int (*)(void *), int, int);
extern void func_08B32370(S31D04 *);
extern int func_088D4C5C(void);
extern void func_08873D34(int, int (*)(void *), float, int, int, int);
extern void func_08873DB0(float *, int);
extern int func_08B32CA8(void *);
extern int func_08B32B0E(void *);
extern int func_08B32760(void *);
void func_08B31D04(S31D04 *self)
{
    float p[4];
    float t;
    float zero;
    if (!func_08885544(D_089D69E8)) {
        int id = 0x26;
        func_088850EC(D_089D69E8, id, func_08B32CA8);
        { int zi = 0; func_08885340(D_089D69E8, zi); }
        { int c = 0x9406; func_088854C4(D_089D69E8, id, c); }
        func_08885388(D_089D69E8);
    } else {
        { int on = 1; int zi = 0; func_088852C4(D_089D69E8, on, zi); }
    }
    self->in->cur = 0;
    func_088D3910(0x26, self->in->page, func_08B32B0E, 0, 1);
    func_08B32370(self);
    {
        In31D04 *in = self->in;
        int one = 1;
        in->open = one;
        t = (float)func_088D4C5C();
    }
    func_08873D34(2, func_08B32760, t, 0, 1, 2);
    zero = 0.0f;
    p[0] = zero; p[1] = zero; p[2] = zero; p[3] = zero;
    func_08873DB0(p, 2);
}
