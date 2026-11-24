/* Creates the update task and stores the five source pointers; returns 0 on success. */
extern "C" {
typedef struct Task { char pad[4]; void *owner; } Task;
typedef struct Obj { unsigned char valid; char pad0[3]; int f4; char pad1[0x28 - 8]; void *s28; void *s2c; void *s30; void *s34; void *s38; } Obj;
extern void func_08B1D980(Obj *);
extern Task *func_088823C4(int, int, int, void *, const char *);
extern void func_08B1DAB0(void *);
extern const char D_08B1FCC0[];
int func_08B1D99C(Obj *self, int a, int b, int c, void *p5, void *p6, void *p7, void *p8, void *p9) {
    Task *t;
    func_08B1D980(self);
    if (a == 0 || p5 == 0 || p6 == 0 || p7 == 0 || p8 == 0 || p9 == 0) return -1;
    t = func_088823C4(a, b, c, (void *)func_08B1DAB0, D_08B1FCC0);
    if (t == 0) return -2;
    t->owner = self;
    self->s28 = p5;
    self->s2c = p6;
    self->s30 = p7;
    self->s34 = p8;
    self->s38 = p9;
    self->f4 = 0;
    self->valid = 1;
    return 0;
}
}
