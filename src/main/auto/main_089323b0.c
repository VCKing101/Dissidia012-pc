/* func_089323B0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_08931D68();
extern void func_089323EC();
extern int D_089CF260;

int func_089323B0(int param_1)

{
  func_08931D68();
  *(char **)(param_1 + 0xc) = &D_089CF260;
  func_089323EC(param_1);
  return param_1;
}
