/* func_088953F0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08866AA8(unsigned char param_1,unsigned char param_2,unsigned char param_3);

void func_088953F0(int param_1)

{
  unsigned int uVar1;
  
  uVar1 = *(unsigned int *)(param_1 + 0x100);
  func_08866AA8(uVar1 & 0xff,uVar1 >> 8 & 0xff,uVar1 >> 0x10 & 0xff);
  return;
}
