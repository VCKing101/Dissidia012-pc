/* func_08973E20: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

int func_08973E20(int *param_1,int param_2,int param_3,int *param_4)

{
  int iVar1;
  int *puVar2;
  
  iVar1 = **(int **)(*param_1 + param_2 * 4);
  puVar2 = (int *)(iVar1 + *(int *)(iVar1 + 0x20) + param_3 * 8);
  *param_4 = *puVar2;
  return iVar1 + puVar2[1];
}
