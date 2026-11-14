/* func_08890B04: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08890B9C();
extern void func_08992F04();
extern int D_089CA748;

void func_08890B04(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x24) = &D_089CA748;
    func_08890B9C(param_1);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
