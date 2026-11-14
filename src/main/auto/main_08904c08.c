/* func_08904C08: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_08904BE0();
extern int * func_08905320();
extern void func_08993560();
extern int D_089CDBC0;

int * func_08904C08(int *param_1,int param_2)

{
  *param_1 = param_2;
  param_1[1] = 0;
  param_1[2] = &D_089CDBC0;
  param_1[3] = param_1;
  param_1[4] = 0;
  func_08905320(param_1 + 5);
  func_08993560(param_1 + 0xd,0x10,0x1c,func_08904BE0);
  param_1[0x7e] = 0;
  return param_1;
}
