/* func_088B3830: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088B38C0(int *param_1);
extern int D_089CB7C8;

int func_088B3830(int param_1)

{
  *(char **)(param_1 + 0x1c) = &D_089CB7C8;
  func_088B38C0(param_1);
  return param_1;
}
