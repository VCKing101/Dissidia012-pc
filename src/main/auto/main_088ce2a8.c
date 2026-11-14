/* func_088CE2A8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088CDEE0(int param_1);
extern void func_08992F6C(int *param_1);
extern int D_089D7820;
extern int D_09AF0274;
extern int D_09AF0278;

char * func_088CE2A8(void)

{
  if (D_09AF0274 == 0) {
    D_09AF0274 = 1;
    func_088CDEE0(&D_09AF0278);
    func_08992F6C(&D_089D7820);
  }
  return &D_09AF0278;
}
