/* Clears the screen tint, then resets four sub-objects of the battle object. */
extern void func_08AEEF4C(void *obj, float *col, int a);
extern void func_08B16EB0(void *p);
extern void func_08B1CE74(int g);
extern void func_08B096AC(void *p);
extern void func_08B121DC(void *p);
extern char D_09B49140[];
extern int D_08B1D820;
void func_08B05D34(char *self) {
    float col[4];
    col[0] = 0.0f;
    col[1] = 0.0f;
    col[2] = 0.0f;
    col[3] = 0.0f;
    func_08AEEF4C(D_09B49140 + 0x50, col, 0);
    func_08B16EB0(self + 0x60);
    func_08B1CE74(D_08B1D820);
    func_08B096AC(self + 0xA4);
    func_08B121DC(self + 0x388C);
}
