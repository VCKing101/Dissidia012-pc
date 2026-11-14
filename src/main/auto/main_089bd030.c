/* func_089BD030: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile). Names are placeholders. */

unsigned int func_089BD030(int param_1,int param_2)

{
  unsigned int uVar1;
  
  uVar1 = 0;
  if (param_1 != 0) {
    uVar1 = *(unsigned char *)(param_1 + 0x18) - 1;
    uVar1 = param_2 + uVar1 & ~uVar1;
  }
  return uVar1;
}
