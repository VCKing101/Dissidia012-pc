/* func_088CDF18: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088CDF80(int param_1);
extern void func_08992F04(int param_1);
extern int D_089CBD50;

void func_088CDF18(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0xc) = &D_089CBD50;
    func_088CDF80(param_1);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
