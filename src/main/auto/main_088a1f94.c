/* func_088A1F94: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_088A1F94(int param_1,unsigned char param_2)

{
  unsigned int uVar1;
  
  if (param_2 == '\x01') {
    uVar1 = *(unsigned int *)(param_1 + 0x34) | 0x20;
  }
  else {
    uVar1 = *(unsigned int *)(param_1 + 0x34) & 0xffffffdf;
  }
  *(unsigned int *)(param_1 + 0x34) = uVar1;
  return;
}
