/* func_089424A4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern char * func_08935828();
extern int func_089748A4();

void func_089424A4(int *param_1,int param_2)

{
  int uVar1;
  int iVar2;
  
  iVar2 = param_2 << 2;
  param_1[5] = param_2;
  uVar1 = func_089748A4(iVar2);
  *param_1 = uVar1;
  func_08935828(uVar1,0,iVar2);
  uVar1 = func_089748A4(iVar2);
  param_1[2] = uVar1;
  func_08935828(uVar1,0,iVar2);
  uVar1 = func_089748A4(iVar2);
  param_1[3] = uVar1;
  func_08935828(uVar1,0,iVar2);
  uVar1 = func_089748A4(iVar2);
  param_1[4] = uVar1;
  func_08935828(uVar1,0,iVar2);
  return;
}
