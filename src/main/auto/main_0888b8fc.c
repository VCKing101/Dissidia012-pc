/* func_0888B8FC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0886503C(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4);
extern void func_0886504C(unsigned int *param_1,unsigned int param_2,unsigned int param_3);
extern void func_0888B858(void);
extern int D_0888B894;

void func_0888B8FC(int param_1,unsigned char param_2)

{
  func_0886503C(param_1 + 0x100,func_0888B858,&D_0888B894,0);
  func_0886504C(param_1 + 0x100,param_1,0x100);
  *(unsigned int *)(param_1 + 0x128) = 0xffffffff;
  *(unsigned int *)(param_1 + 300) = 0xffffffff;
  *(unsigned char *)(param_1 + 0x134) = param_2;
  return;
}
