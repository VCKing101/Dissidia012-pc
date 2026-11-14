/* func_088A952C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088A7FB8(void);
extern void func_088A9B18(int param_1);

void func_088A952C(int *param_1)

{
  int iVar1;
  
  iVar1 = 0;
  while( 1 ) {
    if (iVar1 == 0) {
      iVar1 = *param_1;
    }
    else {
      iVar1 = *(int *)(iVar1 + 0x34);
    }
    if (iVar1 == 0) break;
    func_088A9B18(iVar1);
  }
  param_1[1] = 0;
  *param_1 = 0;
  func_088A7FB8();
  return;
}
