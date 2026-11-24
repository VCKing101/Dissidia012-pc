/* Frees the object's buffer, resets it, then forwards the saved battle setup in D_09B6CDA0 to func_08AF2D08 and returns 1. */
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
typedef struct { char p0[0x138]; int p138; int p13C; } Obj;
extern Glob D_09B6CDA0;
extern int D_08B01CF8;
extern void func_0885C538(int a, int b, int c);
extern void func_08AF2400(Obj *o);
extern void func_08AF2CB0(int a);
extern void func_08AF2D08(int a, void *b, int c, int d, int e, int f, int g, int h, int i, int j, void *k, int l, int m, int n, int o);
int func_08AF251C(Obj *self) {
    func_0885C538(self->p138, self->p13C, 0);
    self->p13C = 0;
    func_08AF2400(self);
    func_08AF2CB0(D_08B01CF8);
    func_08AF2D08(D_08B01CF8, (char *)&D_09B6CDA0 + 8, D_09B6CDA0.w0C8, D_09B6CDA0.w0CC, D_09B6CDA0.w0D0, D_09B6CDA0.b0D4, 0, D_09B6CDA0.w0, 1, -1, D_09B6CDA0.blk, D_09B6CDA0.b9BC, D_09B6CDA0.w9C4, D_09B6CDA0.w9C8, D_09B6CDA0.b5);
    return 1;
}
