/* func_08930158: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D3CF0();
extern int func_088DAA68();
extern void func_089301D0();
extern int D_089CF138;

void func_08930158(int param_1)

{
  int uVar1;
  int iVar2;
  int iVar3;
  
  func_089301D0();
  iVar3 = 0;
  iVar2 = param_1;
  do {
    uVar1 = func_088DAA68(*(int *)(param_1 + 0x2c));
    *(int *)(iVar2 + 0x30) = uVar1;
    iVar3 = iVar3 + 1;
    iVar2 = iVar2 + 4;
  } while (iVar3 < 1);
  func_088D3CF0(0,param_1 + 0x30,&D_089CF138);
  *(short *)(*(int *)(param_1 + 0x30) + 0xe) = 1;
  return;
}
