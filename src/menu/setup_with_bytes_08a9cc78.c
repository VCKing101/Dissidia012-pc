/* Runs the setup with mode 0, copies a 0x1A-byte block and a six-byte record into the global block. */
typedef struct Six { char c[6]; } Six;
extern int D_08AA3F44;
extern struct Blk { char g0[8]; char small[0x1A]; char g1[0x1D4 - 0x22]; Six six; } D_09B6EFB0;
extern void func_08A97E34(int obj, int a, int b, int c, int d);
extern void func_08935524(void *d, const void *s, int n);
void func_08A9CC78(int a, int b, int c, int d, int e, Six *p) {
    Six pad;
    Six v;
    pad.c[0] = 0;
    v = *p;
    func_08A97E34(D_08AA3F44, 3, 0, c, 0);
    func_08935524(D_09B6EFB0.small, (const void *)d, 0x1A);
    D_09B6EFB0.six = v;
}
