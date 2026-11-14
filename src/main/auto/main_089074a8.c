/* func_089074A8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_089074A8(int param_1)

{
  int iVar1;
  
  iVar1 = 0;
  do {
    *(int *)(param_1 + 0x5c) = 0;
    *(int *)(param_1 + 0x60) = 0;
    iVar1 = iVar1 + 1;
    param_1 = param_1 + 0x80;
  } while (iVar1 < 0x10);
  return;
}
