/* func_0892CBB0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0892ABC0();
extern int func_0892C730();
extern void func_0892CC78();
extern void func_08993560();
extern int D_089CED38;

int func_0892CBB0(int param_1)

{
  func_0892ABC0();
  *(char **)(param_1 + 0x24) = &D_089CED38;
  func_08993560(param_1 + 0x38,2,0x28,func_0892C730);
  func_0892CC78(param_1);
  return param_1;
}
