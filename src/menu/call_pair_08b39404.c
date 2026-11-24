/* Calls func_08B3A0E4 with the value at *p and a float constant, then func_08B3C488 with 0x2E. */
extern float D_08B416A8;
extern void func_08B3A0E4(void *self, int v, float f);
extern void func_08B3C488(void *self, int v);
void func_08B39404(void *self, int *p)
{
    func_08B3A0E4(self, *p, D_08B416A8);
    func_08B3C488(self, 0x2E);
}
