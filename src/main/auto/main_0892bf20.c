/* func_0892BF20: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D46E4(int *param_1,int param_2,int param_3,int param_4,
                 int param_5,int param_6);
extern void func_0892B544(int param_1);

void func_0892BF20(int param_1,int param_2,int param_3)

{
  *(int *)(param_1 + 0x19c) = param_2;
  func_088D46E4(param_1 + 0x1a4,param_3,0,*(int *)(param_1 + 0x198) + -1,0,0xffffffff);
  *(unsigned int *)(param_1 + 0x1a4) = *(unsigned int *)(param_1 + 0x1a4) | 1;
  func_0892B544(param_1 + *(int *)(param_1 + 0x1a8) * 0x44 + 0x44);
  *(int *)(param_1 + 0x28) = 2;
  return;
}
