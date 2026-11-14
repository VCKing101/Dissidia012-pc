/* func_0892B874: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0892ABC0();
extern int func_0892B05C();
extern void func_0892B9C8();
extern void func_08993560();
extern int D_089CEC58;

int func_0892B874(int param_1)

{
  func_0892ABC0();
  *(char **)(param_1 + 0x24) = &D_089CEC58;
  func_08993560(param_1 + 0x44,5,0x44,func_0892B05C);
  func_0892B9C8(param_1);
  return param_1;
}
