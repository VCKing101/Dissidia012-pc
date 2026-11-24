// Spawns the object for the entry at +0x1C (if set): stores func_088A3E9C() at +0x28, a second handle at +0x2C, then scales the object's part by (signed byte at +0x30 + 0x12) * 0.01.
extern int func_088A3E9C(void);
extern int func_088A3CAC(int, int, int);
extern void func_088A3D30(int, int, int, int);
extern void func_0889F8D4(float, int, int);
typedef struct Cfg57 {
    char pad0[0x12];
    signed char percent;
} Cfg57;
typedef struct Pair57 {
    int a;
    int pad;
    int b;
} Pair57;
typedef struct Own57 {
    char pad0[0x1C];
    Pair57 pair;
    int obj;
    int handle;
    Cfg57 *cfg;
} Own57;
void func_08B098E4(Own57 *self)
{
    Pair57 *p = &self->pair;
    if (p->a != 0) {
        self->obj = func_088A3E9C();
        if (self->obj != 0) {
            int o = self->obj;
            float scale = (float)self->cfg->percent * 0.01f;
            self->handle = func_088A3CAC(o, p->a, p->b);
            func_088A3D30(self->obj, self->handle, 0, 0);
            func_0889F8D4(scale, self->obj + 4, 0);
        }
    }
}
