/* func_089907F8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int D_089DA420;

int func_089907F8(int param_1)

{
  int iVar1;
  
  iVar1 = D_089DA420 + param_1 * 0x10;
  *(char *)(iVar1 + 0x38) = 0;
  *(int *)(iVar1 + 0x3c) = 0xffffffff;
  *(int *)(iVar1 + 0x40) = 0;
  return *(int *)(iVar1 + 0x34);
}
