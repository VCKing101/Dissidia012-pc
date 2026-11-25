typedef struct V4 { float x, y, z, w; } V4;
typedef struct Pk { float a, b, c, d; char pad[0x30]; float s; } Pk;
extern void func_0895ACA4(Pk *p, int c);
extern int D_09B6F470;
extern void func_0896F320(int h, void *a, void *b, int c);
extern void func_0896F5C0(float f, int h);
void func_0895AA80(int a, int b, int c, Pk **pp) {
    Pk *p = *pp;
    V4 v;
    int h;
    func_0895ACA4(p, c);
    h = D_09B6F470;
    v = *(V4 *)p;
    func_0896F320(h, &v, &v, 0);
    func_0896F5C0(p->s * 0.001953125f, h);
}
