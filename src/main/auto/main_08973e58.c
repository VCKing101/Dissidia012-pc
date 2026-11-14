/* func_08973E58: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

int func_08973E58(int *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = **(int **)(*param_1 + param_2 * 4);
  return iVar1 + *(int *)(iVar1 + 0x24);
}
