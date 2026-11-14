/* func_088D4744: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088D35E0(int param_1,int param_2,int param_3);

void func_088D4744(int param_1)

{
  int uVar1;
  
  uVar1 = func_088D35E0(param_1,*(int *)(param_1 + 8),1);
  *(int *)(param_1 + 0x20) = uVar1;
  uVar1 = func_088D35E0(param_1,*(int *)(param_1 + 0xc),0xffffffff);
  *(int *)(param_1 + 0x24) = uVar1;
  return;
}
