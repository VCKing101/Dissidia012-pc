/* func_08908B58: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0885C538();
extern void func_08905848();
extern void func_089C5970();

void func_08908B58(int param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  if (*(int *)(param_1 + 0x24) != 0) {
    func_0885C538(*(int *)(param_1 + 0x260),*(int *)(param_1 + 0x24),0);
    *(int *)(param_1 + 0x24) = 0;
    *(int *)(param_1 + 0x28) = 0;
  }
  if (*(int *)(param_1 + 0x2c) != 0) {
    func_0885C538(*(int *)(param_1 + 0x260),*(int *)(param_1 + 0x2c),0);
    *(int *)(param_1 + 0x2c) = 0;
    *(int *)(param_1 + 0x30) = 0;
  }
  iVar3 = 0;
  piVar2 = (int *)(param_1 + 0x40);
  iVar1 = param_1 + 0x38;
  do {
    if (*piVar2 != 0) {
      func_0885C538(*(int *)(param_1 + 0x260),*piVar2,0);
    }
    func_08905848(iVar1);
    *(int *)(iVar1 + 8) = 0;
    *(int *)(iVar1 + 0xc) = 0;
    *(char *)(iVar1 + 0x10) = 0;
    iVar3 = iVar3 + 1;
    piVar2 = piVar2 + 8;
    iVar1 = iVar1 + 0x20;
  } while (iVar3 < 0x10);
  func_089C5970(*(int *)(param_1 + 0x24c));
  *(int *)(param_1 + 0x24c) = 0xffffffff;
  return;
}
