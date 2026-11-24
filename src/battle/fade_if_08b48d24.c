/* When func_08873EB4 reports true, sets flag 1, starts a timed effect and sets the tint to (0, 0, 0, 0). */
extern int func_08873EB4(void);
extern void func_08873D34(float t, int a, void *cb, int b, int c, int d);
extern void func_08873DB0(float *col, int a);
extern void func_08B48E08(void);
typedef struct { char pad[0x5C]; unsigned flags; } Obj;
void func_08B48D24(Obj *self) {
    float col[4];
    if (func_08873EB4() != 0) {
        self->flags |= 1;
        func_08873D34(10.0f, 4, (void *)func_08B48E08, 0, 1, 2);
        col[0] = 0.0f;
        col[1] = 0.0f;
        col[2] = 0.0f;
        col[3] = 0.0f;
        func_08873DB0(col, 2);
    }
}
