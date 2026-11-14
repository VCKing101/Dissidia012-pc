/* func_088A9BF8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

unsigned char func_088A9BF8(int param_1)

{
  unsigned char bVar1;
  
  bVar1 = 1;
  if (*(unsigned char *)(param_1 + 0x30) == '\x01') {
    bVar1 = *(int *)(param_1 + 0x1c) == 0;
  }
  return bVar1;
}
