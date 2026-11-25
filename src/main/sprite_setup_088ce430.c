/* Sets up a sprite from its source: -1 for no source, -2 when the object cannot be created, -3 when the source data is rejected, 0 on success. */
typedef struct S { char p0[4]; int obj; } S;
extern int D_089D7830;
extern void func_088CE3D8(S *self);
extern int func_088D0700(int heap, int a, int b, int c);
extern int func_088CE8C0(S *self, void *src, int a, int b);
int func_088CE430(S *self, void *src, int a, int b, int c, int d, int e)
{
    int cc;
    func_088CE3D8(self);
    if (src == 0) {
        return -1;
    }
    cc = (unsigned char)c;
    self->obj = func_088D0700(D_089D7830, cc & 0xFF, d, e);
    if (self->obj == 0) {
        return -2;
    }
    if (func_088CE8C0(self, src, a, b) != 0) {
        return -3;
    }
    return 0;
}
