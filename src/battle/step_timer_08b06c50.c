/* Advances the timer at +0x2C by the frame step until it reaches 354, then flags the battle and switches state; returns 1 when done. */
typedef struct FrameClock { char pad[0x18]; float step; } FrameClock;
typedef struct Obj { char pad0[0x2C]; float t; int arg; char pad1[0x64]; int state; } Obj;
extern FrameClock *D_089D69E0;
extern unsigned char D_089D6C98;
extern void func_08B08A1C(float t, Obj *self, int arg);
extern void func_0888A578(void);

int func_08B06C50(Obj *self)
{
    float t;
    int arg = self->arg;
    func_08B08A1C(self->t, self, arg);
    t = self->t;
    if (!(t < 354.0f)) {
        D_089D6C98 = 1;
        self->state = 12;
        func_0888A578();
        return 1;
    }
    self->t = t + D_089D69E0->step;
    return 0;
}
