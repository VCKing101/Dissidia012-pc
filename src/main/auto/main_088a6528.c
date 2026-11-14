/* func_088A6528: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088A65E4(int *param_1);
extern void func_08992F04(int param_1);

void func_088A6528(int param_1,unsigned int param_2)

{
  if ((param_1 != 0) && (func_088A65E4(param_1), (param_2 & 1) != 0)) {
    func_08992F04(param_1);
  }
  return;
}
