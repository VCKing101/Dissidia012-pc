/* func_088BD518: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088BD564(int *param_1);
extern void func_08992F04(int param_1);

void func_088BD518(int param_1,unsigned int param_2)

{
  if ((param_1 != 0) && (func_088BD564(param_1), (param_2 & 1) != 0)) {
    func_08992F04(param_1);
  }
  return;
}
