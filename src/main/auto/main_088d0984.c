/* func_088D0984: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088D0684(int param_1,unsigned char param_2,unsigned int param_3);
extern void func_088DA924(int param_1,unsigned int param_2);
extern void func_088DB10C(int param_1);

void func_088D0984(unsigned int param_1,unsigned int param_2,unsigned char param_3,unsigned short param_4)

{
  int iVar1;
  
  iVar1 = func_088D0684(param_1,param_3,param_4);
  if (iVar1 != 0) {
    func_088DB10C(param_2);
    func_088DA924(param_2,iVar1);
  }
  return;
}
