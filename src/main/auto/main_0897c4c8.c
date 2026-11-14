/* func_0897C4C8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_089A05F0(unsigned int param_1,unsigned int param_2,unsigned int param_3,int param_4,unsigned int param_5,unsigned int param_6);

void func_0897C4C8(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 8);
  func_089A05F0(4,0x19f,6,3,0,*(int *)(param_1 + iVar1 * 4));
  *(int *)(param_1 + 8) = 1 - iVar1;
  return;
}
