/* Initialises an object: creates a handle at +0x18 and three further handles derived from it, then registers them. */
typedef struct { char pad[0x18]; int h; int more[3]; } Obj;
extern void func_08B47808(void *self);
extern int func_0887B3EC(int a, int b, int c, int d, int e, int f);
extern int func_0887E534(int h);
extern void func_088D428C(void *a, void *b, void *c, int d);
extern char D_08B48490[];
void func_08B47764(Obj *self, int a1, int a2, int a3) {
    int i;
    int *p;
    func_08B47808(self);
    self->h = func_0887B3EC(a1, a2, 0, a3, 0, 0);
    i = 1;
    p = (int *)((char *)self + 4);
    do {
        p[6] = func_0887E534(self->h);
        i++;
        p++;
    } while (i < 4);
    func_088D428C((char *)self + 8, &self->h, D_08B48490, 1);
}
