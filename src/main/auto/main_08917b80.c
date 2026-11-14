/* func_08917B80: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int D_089D83D8;

void func_08917B80(int param_1)

{
  int iVar1;
  
  iVar1 = D_089D83D8;
  *(int *)(D_089D83D8 + 0x4c) = param_1;
  *(int *)(iVar1 + 0x54) = 0;
  *(int *)(iVar1 + 0x58) = 0xffffffff;
  *(int *)(iVar1 + 0x38) = 0xffffffff;
  *(int *)(iVar1 + 0x5c) = 0;
  return;
}
