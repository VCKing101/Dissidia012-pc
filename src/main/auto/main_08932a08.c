/* func_08932A08: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0892ABC0();
extern int func_08931E7C();
extern int func_089323B0();
extern void func_08932AF4();
extern int D_089CF290;

int func_08932A08(int param_1)

{
  func_0892ABC0();
  *(char **)(param_1 + 0x24) = &D_089CF290;
  func_08931E7C(param_1 + 0x38);
  func_089323B0(param_1 + 0x5c);
  func_08932AF4(param_1);
  return param_1;
}
