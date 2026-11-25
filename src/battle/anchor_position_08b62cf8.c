// Picks a camera-anchor position: the object's own position when it exists, otherwise a computed one.
typedef struct V4 { float x, y, z, w; } V4;
extern int func_08842164(void);
extern V4 *func_0885EA90(int a);
extern V4 *func_0885EA98(int a);
extern float func_0887357C(int a);
extern void func_089A38FC(V4 *d, V4 *a, V4 *b);
extern void func_089A3BC8(V4 *d, V4 *s);
extern void func_089A3A64(V4 *d, V4 *s, float f);
extern void func_089A38C4(V4 *d, V4 *a, V4 *b);
extern int D_089D5EC0;
extern const float D_08B62ED0, D_08B62ED4, D_08B62ED8;

void func_08B62CF8(V4 *out)
{
    int iVar1 = func_08842164();
    if (iVar1 != 0) {
        *out = *(V4 *)(iVar1 + 0x80);
    } else {
        int g = D_089D5EC0 + 0x720;
        int h = D_089D5EC0 + 0x7E0;
        V4 a, b, c;
        float f, d;
        a = *func_0885EA90(g);
        b = *func_0885EA98(g);
        f = func_0887357C(h);
        func_089A38FC(&c, &b, &a);
        func_089A3BC8(&c, &c);
        func_089A3A64(&c, &c, f + D_08B62ED0 * 0.5f);
        func_089A38C4(&a, &a, &c);
        d = D_08B62ED8 - D_08B62ED4;
        a.y = a.y - d * 0.5f;
        *out = a;
        return;
    }
}
