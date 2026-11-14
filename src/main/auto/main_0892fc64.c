/* func_0892FC64: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D3CF0();
extern int func_088DAA68();
extern void func_0892FCA8();
extern int D_089CF080;

void func_0892FC64(int param_1)

{
  int uVar1;
  
  func_0892FCA8();
  uVar1 = func_088DAA68(*(int *)(param_1 + 0x2c));
  *(int *)(param_1 + 0x34) = uVar1;
  func_088D3CF0(0,param_1 + 0x34,&D_089CF080);
  return;
}
