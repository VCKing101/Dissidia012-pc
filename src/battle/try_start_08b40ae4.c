/* If func_08B3D9C4 returns 1: builds a vector from the two points, stores func_088686FC of it at +0xC and +0x10, zeroes +0x14; returns whether it ran. */
struct Obj { char pad[0xC]; float a; float b; float c; };
extern int func_08B3D9C4(void);
extern void func_089A38FC(float *, void *, void *);
extern float func_088686FC(float *);
int func_08B40AE4(struct Obj *self, char *p)
{
    float v[4];
    int r = func_08B3D9C4();
    if (r == 1) {
        float f;
        func_089A38FC(v, p, p + 0x40);
        f = func_088686FC(v);
        self->a = f;
        self->b = f;
        self->c = 0.0f;
        return 1;
    }
    return 0;
}
