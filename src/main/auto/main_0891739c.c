/* func_0891739C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_089132A4();
extern int D_089D83D8;

void func_0891739C(int param_1)

{
  int uVar1;
  
  *(int *)(D_089D83D8 + 0x38) = param_1;
  uVar1 = func_089132A4();
  *(int *)(D_089D83D8 + 0x3c) = uVar1;
  return;
}
