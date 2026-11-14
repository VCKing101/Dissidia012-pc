/* func_0892F384: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0892ABC0();
extern int func_0892EB7C();
extern void func_0892F434();
extern void func_08993560();
extern int D_089CF010;

int func_0892F384(int param_1)

{
  func_0892ABC0();
  *(char **)(param_1 + 0x24) = &D_089CF010;
  func_08993560(param_1 + 0x3c,1,0x10c,func_0892EB7C);
  func_0892F434(param_1);
  return param_1;
}
