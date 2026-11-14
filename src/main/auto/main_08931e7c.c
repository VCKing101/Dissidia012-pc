/* func_08931E7C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_08931D68();
extern void func_08931EB8();
extern int D_089CF230;

int func_08931E7C(int param_1)

{
  func_08931D68();
  *(char **)(param_1 + 0xc) = &D_089CF230;
  func_08931EB8(param_1);
  return param_1;
}
