/* Creates a handle from a global and the argument, positions it from a looked-up vector and sets its scale. */
typedef struct V3 { float x, y, z; } V3;
typedef struct D { char pad[8]; void *h; } D;
typedef struct K { char pad[0x10]; void *x; } K;
typedef struct S { char pad0[0xC4]; K *k; char pad1[0xD0 - 0xC8]; D *d; } S;
extern void *func_08B45EF8(void *);
extern void *func_08B421FC(void *, int);
extern void func_089216F4(V3 *, void *);
extern void func_088A7284(void *, float, float, float);
extern void func_088A6CB8(void *, float, int);
extern void *D_08B42288;
extern float D_08B416A4;
void func_08B3C488(S *self, int arg)
{
    void *e = func_08B45EF8(self->k->x);
    V3 v;
    V3 r;
    void *h;
    self->d->h = func_08B421FC(D_08B42288, arg);
    h = self->d->h;
    func_089216F4(&r, e);
    v = r;
    func_088A7284(h, v.x, v.y, v.z);
    func_088A6CB8(self->d->h, 1.0f, (int)D_08B416A4);
}
