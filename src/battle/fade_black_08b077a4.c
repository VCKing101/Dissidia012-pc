/* Runs three global resets, sets the tint to opaque black, then resets the sub-object at +0xA4. */
extern void func_0888A004(int a);
extern void func_0888A134(void);
extern void func_08ADFA44(void);
extern void func_08AEEF4C(void *obj, float *col, int a);
extern void func_08B09844(void *p);
extern char D_09B49140[];
void func_08B077A4(char *self) {
    float col[4];
    func_0888A004(0);
    func_0888A134();
    func_08ADFA44();
    col[0] = 0.0f;
    col[1] = 0.0f;
    col[2] = 0.0f;
    col[3] = 255.0f;
    func_08AEEF4C(D_09B49140 + 0x50, col, 0);
    func_08B09844(self + 0xA4);
}
