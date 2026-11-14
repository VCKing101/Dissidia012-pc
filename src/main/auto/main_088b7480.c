/* func_088B7480: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_088B7480(int param_1)

{
  int iVar1;
  
  iVar1 = 0;
  do {
    *(short *)(param_1 + 0xa2) = 0xffff;
    iVar1 = iVar1 + 1;
    param_1 = param_1 + 2;
  } while (iVar1 < 10);
  return;
}
