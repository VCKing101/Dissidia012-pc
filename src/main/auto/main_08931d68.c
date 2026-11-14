/* func_08931D68: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08931DA0(int *param_1);
extern int D_089CF200;

int func_08931D68(int param_1)

{
  *(char **)(param_1 + 0xc) = &D_089CF200;
  func_08931DA0(param_1);
  return param_1;
}
