/* func_089B0DAC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile). Names are placeholders. */

void func_089B0DAC(int param_1,unsigned int param_2,unsigned int param_3)

{
  if (param_1 != 0) {
    *(unsigned int *)(param_1 + 0x30) = *(unsigned int *)(param_1 + 0x30) & ~param_2 | param_2 & param_3;
  }
  return;
}
