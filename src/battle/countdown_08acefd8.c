/* Counts the timer at +8 down by the frame step; when ready or expired runs func_08ACEF94. Returns -1 when func_08ACF05C fails. */
typedef struct FrameClock { char pad[0x18]; float step; } FrameClock;
typedef struct Obj { int pad0; int locked; float t; } Obj;
extern FrameClock *D_089D69E0;
extern int func_08ACF05C(Obj *self);
extern int func_08ACF084(void);
extern void func_08ACEF94(Obj *self);

int func_08ACEFD8(Obj *self)
{
    Obj *keep[1];
    int r;
    float t;
    keep[0] = self;
    r = func_08ACF05C(self);
    self = keep[0];
    if (r == 0) {
        return -1;
    }
    if (self->locked == 0) {
        float zero = 0.0f;
        t = self->t - D_089D69E0->step;
        self->t = t;
        if (t <= zero || (keep[0] = self, r = func_08ACF084(), self = keep[0], r == 1)) {
            func_08ACEF94(self);
        }
    }
    return 0;
}
