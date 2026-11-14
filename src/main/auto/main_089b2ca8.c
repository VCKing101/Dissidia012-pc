/* func_089B2CA8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile). Names are placeholders. */
extern int D_089D21B8;

short * func_089B2CA8(int param_1)

{
  if (-1 < **(short **)(param_1 + 0xc0)) {
    return *(short **)(param_1 + 0xc0) + 8;
  }
  return (short *)&D_089D21B8;
}
