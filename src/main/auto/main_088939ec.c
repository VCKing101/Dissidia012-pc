/* func_088939EC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

unsigned char func_088939EC(int param_1,int param_2)

{
  return (*(unsigned int *)(*(int *)(*(int *)(param_1 + 0x74) + 0x108) + param_2 * 0xc + 8) & 0x40000) != 0;
}
