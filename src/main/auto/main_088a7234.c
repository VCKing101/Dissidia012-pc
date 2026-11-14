/* func_088A7234: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088A4A30(int param_1);
extern int func_088A751C(int param_1);

void func_088A7234(int param_1)

{
  int iVar1;
  
  if ((*(int *)(param_1 + 0xd4) == 3) && (iVar1 = func_088A751C(param_1), iVar1 == 1)) {
    func_088A4A30(*(int *)(param_1 + 0xcc));
    *(int *)(param_1 + 0xd4) = *(int *)(param_1 + 0xd8);
  }
  return;
}
