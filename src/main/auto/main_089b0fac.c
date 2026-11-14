/* func_089B0FAC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile). Names are placeholders. */

void func_089B0FAC(int param_1,int param_2)

{
  if (param_1 != 0) {
    *(int *)(param_1 + 0x24) = param_2;
    *(unsigned short *)(param_1 + 2) = *(unsigned short *)(param_1 + 2) | 0x800;
  }
  return;
}
