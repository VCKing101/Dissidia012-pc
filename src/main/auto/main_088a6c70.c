/* func_088A6C70: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088A44F8();
extern int func_088A751C();

void func_088A6C70(int param_1)

{
  int iVar1;
  
  iVar1 = func_088A751C();
  if (iVar1 == 1) {
    func_088A44F8(*(int *)(param_1 + 0xcc),0);
    *(int *)(param_1 + 0xcc) = 0;
    *(int *)(param_1 + 0xd4) = 2;
  }
  return;
}
