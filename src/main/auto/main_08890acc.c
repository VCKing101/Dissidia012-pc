/* func_08890ACC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08890B5C(char *param_1);
extern int D_089CA748;

int func_08890ACC(int param_1)

{
  *(char **)(param_1 + 0x24) = &D_089CA748;
  func_08890B5C(param_1);
  return param_1;
}
