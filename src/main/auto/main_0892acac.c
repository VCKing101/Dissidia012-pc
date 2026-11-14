/* func_0892ACAC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088825D4();
extern int func_0888274C();
extern void func_0892AE80();

void func_0892ACAC(int *param_1)

{
  int iVar1;
  
  if (*param_1 != 0) {
    iVar1 = func_0888274C(param_1[1],func_0892AE80);
    if (iVar1 != 0) {
      func_088825D4(param_1[1],*param_1);
    }
    *param_1 = 0;
    param_1[1] = 0;
  }
  return;
}
