/* Clears D_09B6CDA0+0x9C0 and forwards its arguments to func_08AF2D08 with constants. */
typedef struct { char p0[0x9C0]; int w; } Glob;
extern Glob D_09B6CDA0;
extern void func_08AF2D08(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o);
void func_08AF2F1C(int a0, int a1, int a2, int a3, int a4, int a5, unsigned char a6, unsigned char a7) {
    D_09B6CDA0.w = 0;
    func_08AF2D08(a0, a2, a3, a4, a5, a6, a7, 2, 0, a1, 0, 0, 0, 0, 0);
}
