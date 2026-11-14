/* func_088D38F0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_088D38F0(int *param_1)

{
  int iVar1;
  
  iVar1 = 0;
  do {
    *param_1 = 0;
    iVar1 = iVar1 + 1;
    param_1 = param_1 + 1;
  } while (iVar1 < 0x100);
  return;
}
