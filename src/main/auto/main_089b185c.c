/* func_089B185C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile). Names are placeholders. */

unsigned short func_089B185C(int param_1)

{
  unsigned short uVar1;
  
  uVar1 = 0;
  if (param_1 != 0) {
    uVar1 = *(unsigned short *)(param_1 + 0xe);
  }
  return uVar1;
}
