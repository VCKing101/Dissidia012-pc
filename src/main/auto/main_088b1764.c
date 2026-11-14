/* func_088B1764: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_088B1764(int param_1)

{
  *(char *)(param_1 + 1) = 1;
  if (*(int *)(param_1 + 0x10) == 0) {
    *(int *)(param_1 + 8) = 0;
    *(int *)(param_1 + 0x10) = 3;
  }
  return;
}
