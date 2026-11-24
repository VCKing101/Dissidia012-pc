/* Draws an overlay through the render object when func_08895420 yields a target: sets render state, draws, then restores it. */
extern int func_08899300(void);
extern int func_08895420(int g);
extern int func_089A04E8(void);
extern void func_089A0384(int a);
extern void func_089A119C(int a);
extern int func_088985F0(int a, int b);
extern int func_08893A18(int g);
extern void func_0886C38C(int t, int a, int b, int c, int d, int e);
extern void func_0899FDC4(int s);
extern int D_089D6AC8;
void func_08B608AC(void) {
    int g = D_089D6AC8;
    int h = func_08899300();
    int t = func_08895420(g);
    int saved;
    int w;
    if (t == 0) {
        return;
    }
    saved = func_089A04E8();
    func_089A0384(0x15);
    func_089A119C(1);
    h = func_088985F0(h, 1);
    w = func_08893A18(g);
    func_0886C38C(t, h, 1, 0xFF, w, 0);
    func_089A119C(0);
    func_0899FDC4(saved);
}
