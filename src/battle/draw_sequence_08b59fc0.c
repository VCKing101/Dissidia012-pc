/* Draws three overlay layers (ids 0, 1, 2) with their render states set up and restored. */
extern void *func_08899300(void);
extern void *func_08895420(void *p);
extern void *func_089A04E8(void);
extern void func_089A0384(int a);
extern void func_089A113C(int a, int b, int c);
extern int func_088985F0(void *p, int id);
extern int func_08893A18(void *p);
extern void func_0886C38C(void *a, int b, int c, int d, int e, int f);
extern void func_089A03DC(int a);
extern void func_089A1178(int a);
extern void func_0899FDC4(void *p);
extern void *D_089D6AC8;

void func_08B59FC0(void)
{
    void *s2 = D_089D6AC8;
    void *s1 = func_08899300();
    void *s0 = func_08895420(s2);
    void *s3;
    int s4;
    if (s0 == 0) {
        return;
    }
    s3 = func_089A04E8();
    func_089A0384(0x15);
    func_089A113C(6, 4, 0xFF);
    s4 = func_088985F0(s1, 0);
    func_0886C38C(s0, s4, 1, 0xFF, func_08893A18(s2), 0);
    func_089A0384(0);
    s4 = func_088985F0(s1, 1);
    func_0886C38C(s0, s4, 1, 0xFF, func_08893A18(s2), 0);
    func_089A03DC(0);
    func_089A0384(1);
    func_089A1178(2);
    s4 = func_088985F0(s1, 2);
    func_0886C38C(s0, s4, 1, 0xFF, func_08893A18(s2), 0);
    func_089A1178(7);
    func_0899FDC4(s3);
}
