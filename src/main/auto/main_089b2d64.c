/* func_089B2D64: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile). Names are placeholders. */

void func_089B2D64(int *param_1,unsigned int param_2,int param_3,int param_4)

{
  unsigned int *puVar1;
  
  puVar1 = (unsigned int *)*param_1;
  *param_1 = (int)(puVar1 + 1);
  *puVar1 = param_4 << 0x10 | param_3 << 8 | param_2 | 0xdb000000;
  return;
}
