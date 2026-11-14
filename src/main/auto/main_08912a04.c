/* func_08912A04: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int * func_089A34E0(int *param_1);

void func_08912A04(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = param_1 + param_2 * 0xc;
  func_089A34E0(iVar1 + 0x1d8);
  func_089A34E0(iVar1 + 0x208);
  *(short *)(param_1 + param_2 * 2 + 0x238) = 0;
  *(char *)(param_1 + param_2 + 0x1d4) = 0;
  return;
}
