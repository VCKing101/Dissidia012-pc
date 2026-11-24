/* Forwards the saved battle setup in D_09B6CDA0 to func_08AF2D08 (flag = a1 == 0) after func_08AF2CB0; returns 1. */
typedef struct {
    int w0;
    char b4;
    unsigned char b5;
    char p0[0xC8 - 6];
    int w0C8;
    int w0CC;
    int w0D0;
    unsigned char b0D4;
    char p1[0xF0 - 0xD5];
    char blk[0x9BC - 0xF0];
    unsigned char b9BC;
    char p2[0x9C4 - 0x9BD];
    int w9C4;
    int w9C8;
} Glob;
extern Glob D_09B6CDA0;
extern int D_08B01CF8;
extern void func_08AF2CB0(int a);
extern void func_08AF2D08(int a, void *b, int c, int d, int e, int f, int g, int h, int i, int j, void *k, int l, int m, int n, int o);
int func_08AF2470(int a0, int a1) {
    int flag = 1;
    if (a1 != 0) {
        flag = 0;
    }
    func_08AF2CB0(D_08B01CF8);
    func_08AF2D08(D_08B01CF8, (char *)&D_09B6CDA0 + 8, D_09B6CDA0.w0C8, D_09B6CDA0.w0CC, D_09B6CDA0.w0D0, D_09B6CDA0.b0D4, 0, D_09B6CDA0.w0, flag, -1, D_09B6CDA0.blk, D_09B6CDA0.b9BC, D_09B6CDA0.w9C4, D_09B6CDA0.w9C8, D_09B6CDA0.b5);
    return 1;
}
