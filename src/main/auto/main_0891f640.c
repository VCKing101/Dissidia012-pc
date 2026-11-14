/* func_0891F640: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int D_089D726C;

unsigned char func_0891F640(int param_1)

{
  unsigned int uVar2;
  
  uVar2 = (unsigned int)*(short *)(param_1 + 0xc);
  if (uVar2 == 0xffffffff) {
    return 0;
  }
  else {
    return (*(unsigned int *)(D_089D726C + (uVar2 >> 5) * 4) & 1 << (uVar2 & 0x1f)) != 0;
  }
}
