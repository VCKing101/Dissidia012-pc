/* func_089691A8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int * func_08969F74();
extern int D_089DC4E8;

int func_089691A8(int param_1)

{
  func_08969F74();
  *(char **)(param_1 + 0x20) = &D_089DC4E8;
  *(int *)(param_1 + 0x24) = 0;
  *(int *)(param_1 + 0x28) = 0;
  return param_1;
}
