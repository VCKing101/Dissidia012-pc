/* func_0892EDBC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088D688C(unsigned int *param_1,char param_2);
extern void func_088D9F5C(int param_1,int param_2,char param_3);
extern void func_0892F024(int param_1);

void func_0892EDBC(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 0x60);
  func_088D688C(param_1 + 0x5c,0);
  if (iVar2 != *(int *)(param_1 + 0x60)) {
    func_0892F024(param_1);
    iVar1 = *(int *)(param_1 + 0x60);
    if (iVar2 + 1 == iVar1) {
      func_088D9F5C(*(int *)(param_1 + 0xc),0xd,1);
    }
    else if (iVar2 + -1 == iVar1) {
      func_088D9F5C(*(int *)(param_1 + 8),0xb,1);
    }
    else if (iVar1 < iVar2) {
      func_088D9F5C(*(int *)(param_1 + 0xc),0xd,1);
    }
    else if (iVar2 < iVar1) {
      func_088D9F5C(*(int *)(param_1 + 8),0xb,1);
    }
  }
  return;
}
