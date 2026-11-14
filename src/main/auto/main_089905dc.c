/* func_089905DC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_08992780();
extern void func_089C5FC0();
extern void func_089C5FD8();
extern int D_089DA420;

int func_089905DC(int param_1,int param_2,int param_3)

{
  int iVar1;
  int uVar2;
  
  func_089C5FD8(*(int *)(D_089DA420 + 0x900),1,0);
  uVar2 = 0;
  iVar1 = D_089DA420;
  if (*(int *)(D_089DA420 + 0x8fc) != 0) {
    uVar2 = func_08992780(*(int *)(D_089DA420 + 0x8fc),param_1,param_2,param_3);
    iVar1 = D_089DA420;
    *(char *)(*(int *)(D_089DA420 + 0x8fc) + 0x40) = 1;
  }
  func_089C5FC0(*(int *)(iVar1 + 0x900),1);
  return uVar2;
}
