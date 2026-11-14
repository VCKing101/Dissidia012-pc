/* func_0888E6A8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int * func_08864FF0(int *param_1,int param_2,int param_3,char param_4);
extern int D_089CA5B0;

int func_0888E6A8(int param_1)

{
  func_08864FF0(param_1 + 0x100,1,70000,0x46);
  *(int *)(param_1 + 0x134) = 0;
  *(char **)(param_1 + 0x13c) = &D_089CA5B0;
  *(int *)(param_1 + 0x140) = param_1;
  return param_1;
}
