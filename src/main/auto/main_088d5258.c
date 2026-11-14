/* func_088D5258: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088D51D0(int param_1);

int func_088D5258(int param_1)

{
  int iVar1;
  
  iVar1 = func_088D51D0(param_1 + 0x20);
  if ((iVar1 < 0x22) && (*(short *)(param_1 + 6) == 0)) {
    if (iVar1 < 0xf) {
      iVar1 = iVar1 + 0xf;
    }
    else {
      iVar1 = iVar1 + 1;
    }
  }
  return iVar1;
}
