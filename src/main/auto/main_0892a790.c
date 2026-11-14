/* func_0892A790: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

int func_0892A790(int param_1,int param_2)

{
  int uVar1;
  
  param_1 = param_1 + param_2 * 8;
  uVar1 = *(int *)(param_1 + 0x20);
  *(int *)(param_1 + 0x20) = 0;
  *(int *)(param_1 + 0x24) = 0xffffffff;
  return uVar1;
}
