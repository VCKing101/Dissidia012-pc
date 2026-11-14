/* func_089010CC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_089010CC(int *param_1)

{
  int iVar1;
  
  *param_1 = 0xffffffff;
  param_1[1] = 0xffffffff;
  param_1[2] = 0xffffffff;
  param_1[4] = 0;
  param_1[3] = 0xffffffff;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[0x10] = 0xffffffff;
  iVar1 = 0;
  do {
    *(char *)((int)param_1 + iVar1 + 0x44) = 1;
    iVar1 = iVar1 + 1;
  } while (iVar1 < 0x23);
  return;
}
