/* func_088C823C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088CB9E8();

int func_088C823C(int param_1)

{
  int iVar1;
  int iVar2;
  
  func_088CB9E8();
  iVar2 = 0;
  iVar1 = param_1;
  do {
    *(int *)(iVar1 + 0xce4) = 0;
    iVar2 = iVar2 + 1;
    iVar1 = iVar1 + 4;
  } while (iVar2 < 0x14);
  iVar2 = 0;
  iVar1 = param_1;
  do {
    *(int *)(iVar1 + 0xd34) = 0;
    iVar2 = iVar2 + 1;
    iVar1 = iVar1 + 4;
  } while (iVar2 < 0x80);
  return param_1;
}
