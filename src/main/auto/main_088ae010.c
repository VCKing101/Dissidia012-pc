/* func_088AE010: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_088AE010(unsigned int param_1,unsigned int *param_2,unsigned int *param_3,unsigned int *param_4,unsigned int *param_5,unsigned int *param_6
                 )

{
  *param_2 = param_1 >> 0x14;
  *param_3 = param_1 >> 0x10 & 0xf;
  *param_4 = param_1 >> 0xb & 0x1f;
  *param_5 = param_1 >> 6 & 0x1f;
  *param_6 = param_1 & 0x3f;
  return;
}
