/* func_08916ED8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088650A4(int param_1);
extern void func_08916BB4(int param_1,int param_2);
extern int func_0891705C(int param_1);
extern void func_0891739C(int param_1);
extern void func_08917B80(int param_1);
extern int D_089D83D8;

void func_08916ED8(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = D_089D83D8;
  func_08916BB4(D_089D83D8,param_1);
  func_088650A4(iVar1 + 4);
  iVar1 = func_0891705C(param_2);
  func_08917B80((int)*(short *)(iVar1 + 0x1b0));
  func_0891739C((int)*(short *)(iVar1 + 0x1b2));
  return;
}
