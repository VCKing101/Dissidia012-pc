typedef struct Blob { int w[67]; } Blob;
extern void func_0892A13C(void *s);
extern int func_0887B500(void *a, int b, int c);
extern void func_0892A198(void *self, int v);
extern void func_0892A17C(void *self);
extern void func_08935524(void *d, void *s, int n);
void func_08929AFC(char *self, short *src, void *p, int q) {
    int v;
    func_0892A13C(self);
    if (src != 0) {
        if (p != 0) {
            v = func_0887B500(p, q, *src);
            func_0892A198(self, v);
        }
        func_0892A17C(self);
        func_08935524(self + 0x1A, src + 2, 0x2A);
        *(Blob *)(self + 0x44) = *(Blob *)(src + 0x18);
    }
}
