/* func_08912644: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_08912644(int param_1)

{
  unsigned int *puVar1;
  
  puVar1 = (unsigned int *)(param_1 + ((unsigned int)(int)*(short *)(param_1 + 0x242) >> 5) * 4 + 0x10);
  *puVar1 = *puVar1 | 1 << ((int)*(short *)(param_1 + 0x242) & 0x1fU);
  return;
}
