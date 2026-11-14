/* func_08922DB0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0885C538();
extern void func_088A6C70();
extern int func_08922C54();
extern int func_08B42B40();

void func_08922DB0(int param_1)

{
  int uVar1;
  int iVar2;
  
  func_08922C54();
  iVar2 = *(int *)(param_1 + 0x20);
  if (*(int *)(iVar2 + 0xc0) != 0) {
    func_088A6C70(*(int *)(iVar2 + 0xc0));
    iVar2 = *(int *)(param_1 + 0x20);
  }
  if (*(int *)(iVar2 + 0xc4) != 0) {
    func_088A6C70(*(int *)(iVar2 + 0xc4));
    iVar2 = *(int *)(param_1 + 0x20);
  }
  if (*(int *)(iVar2 + 200) != 0) {
    func_088A6C70(*(int *)(iVar2 + 200));
    iVar2 = *(int *)(param_1 + 0x20);
  }
  if (*(int *)(iVar2 + 0xcc) != 0) {
    func_088A6C70();
  }
  uVar1 = func_08B42B40();
  func_0885C538(uVar1,*(int *)(param_1 + 0x20),0);
  *(int *)(param_1 + 0x20) = 0;
  return;
}
