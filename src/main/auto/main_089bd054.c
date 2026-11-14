/* func_089BD054: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile). Names are placeholders. */

int func_089BD054(int param_1)

{
  int iVar1;
  
  iVar1 = 0;
  if (param_1 != 0) {
    iVar1 = (int)((unsigned int)*(unsigned char *)(param_1 + 0x17) * (unsigned int)*(unsigned char *)(param_1 + 0x18)) >> 3;
  }
  return iVar1;
}
