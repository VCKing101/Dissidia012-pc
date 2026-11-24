/* Sets flag 2, starts a timed effect and sets the global tint to (255, 255, 255, 255). */
extern void func_08873D34(float t, int a, void *cb, int b, int c, int d);
extern void func_08873DB0(float *col, int a);
extern void func_08B48E24(void);
typedef struct { char pad[0x5C]; unsigned flags; } Obj;
void func_08B48DA0(Obj *self) {
    float col[4];
    self->flags |= 2;
    func_08873D34(10.0f, 3, (void *)func_08B48E24, 0, 1, 2);
    col[0] = 255.0f;
    col[1] = 255.0f;
    col[2] = 255.0f;
    col[3] = 255.0f;
    func_08873DB0(col, 2);
}
