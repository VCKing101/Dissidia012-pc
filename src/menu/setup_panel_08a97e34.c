/* Registers the panel callbacks on self + 0x100 and stores the setup arguments in the global block. */
extern int D_089D4960;
extern struct Blk { int mode; char g0[0x1C8]; int second; int third; char g1[0x120]; unsigned w; char g2[0x1A8]; unsigned char b; } D_09B6EFB0;
extern void func_08844588(int g);
extern void func_0886503C(int p, void *a, void *b, int c);
extern void func_088650A4(int p);
extern void func_0886504C(int p, int owner, int size);
extern void func_08A98128(void);
extern void func_08A98148(void);
void func_08A97E34(int self, int mode, int second, int third, unsigned char b) {
    int p;
    func_08844588(D_089D4960);
    p = self + 0x100;
    func_0886503C(p, (void *)func_08A98128, (void *)func_08A98148, 0);
    func_088650A4(p);
    func_0886504C(p, self, 0x100);
    D_09B6EFB0.mode = mode;
    if (second >= 0) {
        D_09B6EFB0.second = second;
    }
    D_09B6EFB0.third = third;
    D_09B6EFB0.w = D_09B6EFB0.w & 0xFFFFFFFE;
    D_09B6EFB0.b = b;
}
