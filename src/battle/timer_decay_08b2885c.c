// Per-frame timer at +0x10: reset to the global when func_08B239EC reports 1 with byte +0xC clear, reset again if func_08B23D08 reports 1, otherwise counts down by the frame step and clamps at 0. Returns 0.
extern int func_08B239EC(int);
extern int func_08B23D08(int);
extern float D_08B28E58;
typedef struct Step57 {
    char pad0[0x18];
    float step;
} Step57;
extern Step57 *D_089D69E0;
typedef struct Own57 {
    char pad0[0xC];
    unsigned char flag;
    char pad1[3];
    float timer;
    int pad2;
    int ref;
} Own57;
int func_08B2885C(Own57 *self)
{
    if (func_08B239EC(self->ref) == 1 && self->flag == 0) {
        self->timer = D_08B28E58;
    }
    if (func_08B23D08(self->ref) == 1) {
        self->timer = D_08B28E58;
    } else {
        float zero = 0.0f;
        float t = self->timer;
        if (!(t <= zero)) {
            t = t - D_089D69E0->step;
            self->timer = t;
            if (t < zero) {
                self->timer = zero;
            }
        }
    }
    return 0;
}
