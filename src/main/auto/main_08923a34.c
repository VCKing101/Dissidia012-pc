/* func_08923A34: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088A6C70();
extern int func_08B34600();

void func_08923A34(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x20);
  if (*(int *)(iVar1 + 0xbc) != 0) {
    func_08B34600(*(int *)(iVar1 + 0xbc));
    iVar1 = *(int *)(param_1 + 0x20);
  }
  if (*(int *)(iVar1 + 0xc0) != 0) {
    func_088A6C70(*(int *)(iVar1 + 0xc0));
    *(int *)(*(int *)(param_1 + 0x20) + 0xc0) = 0;
    iVar1 = *(int *)(param_1 + 0x20);
  }
  if (*(int *)(iVar1 + 0xc4) != 0) {
    func_088A6C70();
    *(int *)(*(int *)(param_1 + 0x20) + 0xc4) = 0;
  }
  return;
}
