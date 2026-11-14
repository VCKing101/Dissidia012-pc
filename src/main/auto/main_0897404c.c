/* func_0897404C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_0897404C(int param_1)

{
  int iVar1;
  
  for (iVar1 = *(int *)(param_1 + 0x60c); iVar1 != 0; iVar1 = *(int *)(iVar1 + 0x10)) {
    if (*(int *)(iVar1 + 8) != -1) {
      *(int *)(iVar1 + 0xc) = *(int *)(iVar1 + 0xc) + 1;
    }
  }
  return;
}
