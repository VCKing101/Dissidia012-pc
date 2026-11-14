/* func_0892C2E0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D3CF0();
extern int func_088DAA68();
extern void func_0892C390();
extern int D_089CECC8;

void func_0892C2E0(int param_1)

{
  int uVar1;
  
  func_0892C390();
  uVar1 = func_088DAA68(*(int *)(param_1 + 0x2c));
  *(int *)(param_1 + 0x40) = uVar1;
  uVar1 = func_088DAA68(*(int *)(param_1 + 0x2c));
  *(int *)(param_1 + 0x3c) = uVar1;
  func_088D3CF0(0,param_1 + 0x3c,&D_089CECC8);
  *(short *)(*(int *)(param_1 + 0x3c) + 0xe) = 8;
  return;
}
