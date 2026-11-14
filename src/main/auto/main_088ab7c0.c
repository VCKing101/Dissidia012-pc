/* func_088AB7C0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

unsigned char func_088AB7C0(int param_1)

{
  unsigned int uVar1;
  
  if ((*(int *)(param_1 + 0xf08) == 10) && (*(int *)(param_1 + 0xf0c) == 0x3ecc8)) {
    uVar1 = *(unsigned int *)(param_1 + 0xef8) & 0xffffffdf;
  }
  else {
    uVar1 = *(unsigned int *)(param_1 + 0xef8) | 0x20;
  }
  *(unsigned int *)(param_1 + 0xef8) = uVar1;
  return (uVar1 & 0x20) != 0;
}
