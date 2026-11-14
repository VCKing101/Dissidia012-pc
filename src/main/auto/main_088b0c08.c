/* func_088B0C08: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088ADE90();
extern void func_088ADF28();
extern void func_088AFB30();
extern void func_088B0CC8();
extern int D_089D712C;
extern int D_089D7260;

void func_088B0C08(void)

{
  int iVar1;
  
  iVar1 = *(int *)(D_089D7260 + 0x14);
  *(int *)(iVar1 + 0x814) = 5;
  *(int *)(iVar1 + 0xdd4) = 0;
  *(int *)(iVar1 + 0xdac) = 7;
  func_088ADE90(D_089D712C,iVar1 + 0x864,iVar1 + 0x2a94,0);
  func_088B0CC8(iVar1 + 0x864,iVar1 + 0x2a94);
  func_088ADF28(D_089D712C,iVar1 + 0x8e4,0x13,0);
  func_088AFB30(D_089D7260);
  return;
}
