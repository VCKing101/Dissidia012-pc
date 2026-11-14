/* func_088992B0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088991D0(int param_1);
extern void func_08992F6C(int *param_1);
extern int D_089D6C20;
extern int D_09AEF8D4;
extern int D_09AEF8D8;

char * func_088992B0(void)

{
  if (D_09AEF8D4 == 0) {
    D_09AEF8D4 = 1;
    func_088991D0(&D_09AEF8D8);
    func_08992F6C(&D_089D6C20);
  }
  return &D_09AEF8D8;
}
