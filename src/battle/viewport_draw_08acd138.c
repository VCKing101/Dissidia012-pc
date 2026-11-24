/* If the flag at +8 is set: calls func_089A03DC(0x15), creates the texture handle at +4 once, queries three values and blits with func_089A09A8. */
typedef struct Obj { int owner; int tex; unsigned char flag; } Obj;
extern void func_089A03DC(int id);
extern int func_0885C6CC(int owner, int size, int a, int b, int c, int d);
extern void func_089C5AE0(int *a, int *b, int *c, int d);
extern void func_089A09A8(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int tex);

void func_08ACD138(Obj *self)
{
    if (self->flag != 0) {
        int q[3];
        func_089A03DC(0x15);
        if (self->tex == 0) {
            int one = 1;
            self->tex = func_0885C6CC(self->owner, 0x3FC00, 0, 0, one, 0);
        }
        func_089C5AE0(&q[0], &q[1], &q[2], 0);
        func_089A09A8(q[2], 0, 0, 0x1E0, 0x110, q[1], q[0], 0, 0, 0x1E0, self->tex);
        self->flag = 0;
    }
}
