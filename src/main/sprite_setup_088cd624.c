/* Sets up a sprite from its source: -1 for no source, -2 when the object cannot be created, -3 when the source data is rejected, 0 on success. */
typedef struct S { char p0[4]; int obj; } S;
extern int D_089D7830;
extern void func_088CD4C8(S *self);
extern int func_088D0700(int heap, int a, int b, int c);
extern int func_088CDD84(S *self, void *src, int a, int b, int flag);
int func_088CD624(S *self, void *src, int a, int b, int c, int d, int e, unsigned char flag)
{
    func_088CD4C8(self);
    if (src == 0) {
        return -1;
    }
    int cc = (unsigned char)c;
    self->obj = func_088D0700(D_089D7830, cc & 0xFF, d, e);
    if (self->obj == 0) {
        return -2;
    }
    if (func_088CDD84(self, src, a, b, flag) != 0) {
        return -3;
    }
    return 0;
}
