/* func_08986BC4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08986D00(int param_1,int param_2,int param_3,int param_4,
                 int param_5);

void func_08986BC4(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = (*(int *)(param_1 + 0x20) / 3) * 2;
  func_08986D00(param_1,param_2,*(int *)(param_1 + 0x1c),*(int *)(param_1 + 0x44),iVar1
              );
  func_08986D00(param_1,param_2,*(int *)(param_1 + 0x1c),*(int *)(param_1 + 0x48),iVar1
              );
  return;
}
