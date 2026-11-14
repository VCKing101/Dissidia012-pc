/* func_088D09D8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088D0684(int param_1,unsigned char param_2,unsigned int param_3);

unsigned int func_088D09D8(unsigned int param_1,unsigned char param_2,int param_3)

{
  unsigned int uVar1;
  
  uVar1 = 0;
  if (param_3 != -1) {
    uVar1 = func_088D0684(param_1,param_2,param_3);
  }
  return uVar1;
}
