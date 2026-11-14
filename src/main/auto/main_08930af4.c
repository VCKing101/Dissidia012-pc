/* func_08930AF4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0892ABC0();
extern int func_089302F8();
extern void func_08930BE4();
extern void func_08993560();
extern int D_089CF158;

int func_08930AF4(int param_1)

{
  func_0892ABC0();
  *(char **)(param_1 + 0x24) = &D_089CF158;
  func_08993560(param_1 + 0x38,6,0x5c,func_089302F8);
  func_08930BE4(param_1);
  return param_1;
}
