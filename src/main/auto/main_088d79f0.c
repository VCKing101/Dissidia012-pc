/* func_088D79F0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern unsigned char func_088D8700(int param_1);

int func_088D79F0(int param_1)

{
  int iVar1;
  
  iVar1 = func_088D8700(param_1 + 0x4c);
  if (iVar1 == 1) {
    *(int *)(param_1 + 0x10) = 0;
  }
  return 0;
}
