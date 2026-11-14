/* func_088BFF94: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_088BFF94(short *param_1)

{
  int iVar1;
  
  *param_1 = 0xffff;
  param_1[1] = 0;
  iVar1 = 0;
  do {
    param_1[2] = 0;
    iVar1 = iVar1 + 1;
    param_1 = param_1 + 1;
  } while (iVar1 < 4);
  return;
}
