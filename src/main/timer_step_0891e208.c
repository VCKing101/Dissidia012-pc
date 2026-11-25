typedef struct Gl { char pad[0x18]; float step; } Gl;
typedef struct Obj {
    char pad0[0x13C];
    void *target;
    char pad1[0x18C - 0x140];
    float t;
} Obj;

extern const float D_089C9F54;
extern Gl *D_089D69E0;
extern int func_0891E78C(void *t);
extern void func_0891E1EC(Obj *o);
extern void func_0891E70C(void *t);

void func_0891E208(Obj *a0) {
    Obj *self = a0;
    void *tg = self->target;
    if (tg != 0) {
        float t;
        float step = D_089D69E0->step;
        if (func_0891E78C(tg) == 1) {
            func_0891E1EC(self);
            t = self->t;
        } else {
            t = self->t;
            if (!(t < D_089C9F54 * 3.0f)) {
                func_0891E70C(self->target);
                t = self->t;
            }
        }
        float s2 = t; s2 += step; self->t = s2;
    }
}
