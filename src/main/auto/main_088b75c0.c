/* func_088B75C0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_088B75C0(int param_1)

{
  int iVar1;
  
  iVar1 = 0;
  do {
    *(char *)(param_1 + 0xbb) = 0xff;
    *(char *)(param_1 + 0xbc) = 0;
    iVar1 = iVar1 + 1;
    param_1 = param_1 + 2;
  } while (iVar1 < 5);
  return;
}
