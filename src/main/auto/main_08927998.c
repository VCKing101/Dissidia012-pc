/* func_08927998: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0885C488();
extern void func_0885C538();
extern void func_08926094();
extern void func_08927648();

void func_08927998(int param_1)

{
  int uVar1;
  
  func_08926094();
  func_0885C538(*(int *)(param_1 + 4),*(int *)(*(int *)(param_1 + 0x20) + 8),0);
  func_0885C538(*(int *)(param_1 + 4),*(int *)(*(int *)(param_1 + 0x20) + 4),0);
  func_0885C538(*(int *)(param_1 + 4),**(int **)(param_1 + 0x20),0);
  *(int *)(*(int *)(param_1 + 0x20) + 8) = 0;
  *(int *)(*(int *)(param_1 + 0x20) + 4) = 0;
  **(int **)(param_1 + 0x20) = 0;
  func_0885C538(*(int *)(param_1 + 4),*(int *)(param_1 + 0x20),0);
  *(int *)(param_1 + 0x20) = 0;
  uVar1 = *(int *)(param_1 + 4);
  func_08927648(param_1);
  func_0885C488(uVar1);
  return;
}
