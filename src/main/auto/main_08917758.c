/* func_08917758: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_08865190();
extern int func_08912FA4();
extern int func_08914E58();
extern int func_08B48684();
extern int D_089D83D8;

void func_08917758(void)

{
  int uVar1;
  int iVar2;
  
  uVar1 = func_08B48684();
  *(int *)(D_089D83D8 + 0x44) = uVar1;
  iVar2 = func_08912FA4(*(int *)(D_089D83D8 + 0x44));
  *(int *)(D_089D83D8 + 0x38) = (int)*(short *)(iVar2 + 8);
  func_08914E58(D_089D83D8);
  func_08865190(D_089D83D8 + 4);
  return;
}
