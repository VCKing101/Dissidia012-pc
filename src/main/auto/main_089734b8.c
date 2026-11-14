/* func_089734B8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08972EBC(int *param_1,int param_2,int param_3);

void func_089734B8(int *param_1,int param_2,int param_3)

{
  int *piVar1;
  
  piVar1 = (int *)0x0;
  if (param_1[0x315] != 0) {
    piVar1 = param_1 + 400;
  }
  func_08972EBC(*(int *)(*param_1 + param_2 * 4),piVar1,param_3);
  return;
}
