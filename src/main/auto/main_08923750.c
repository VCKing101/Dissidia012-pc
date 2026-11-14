/* func_08923750: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088470B8();

int func_08923750(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(*(int *)(param_1 + 0x20) + 0xbc);
  iVar2 = 0;
  if ((iVar1 != 0) && (iVar1 = func_088470B8((int)**(short **)(iVar1 + 0x2f8)), iVar1 != 0)) {
    iVar2 = (int)*(char *)(iVar1 + 6);
  }
  return iVar2;
}
