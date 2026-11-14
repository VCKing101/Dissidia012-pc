/* func_088963E4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088A24D4(int param_1,int param_2,int param_3,int param_4);

void func_088963E4(int param_1,int param_2,int param_3,int param_4,
                 int param_5)

{
  func_088A24D4(0,param_2,1,0);
  func_088A24D4(0,param_3,1,0);
  func_088A24D4(0,param_4,1,0);
  func_088A24D4(0,param_5,1,0);
  *(int *)(param_1 + 0x3c) = param_4;
  *(int *)(param_1 + 0x40) = param_5;
  return;
}
