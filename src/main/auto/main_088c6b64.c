/* func_088C6B64: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0887A61C(int param_1);
extern void func_088C6C08(int *param_1);
extern int D_089CBC38;

int func_088C6B64(int param_1)

{
  *(char **)(param_1 + 0x28) = &D_089CBC38;
  func_0887A61C(param_1 + 0x18);
  func_088C6C08(param_1);
  return param_1;
}
