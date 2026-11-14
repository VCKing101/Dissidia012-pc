/* func_088AB7A4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

int func_088AB7A4(int param_1,int param_2,int param_3)

{
  if (param_3 != 1) {
    *(char *)(param_1 + 0xec0) = 0;
    *(int *)(param_1 + 0xebc) = param_3;
  }
  return 0;
}
