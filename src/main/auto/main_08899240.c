/* func_08899240: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0889726C();
extern void func_08992F04();
extern void func_0899372C();

void func_08899240(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    func_0899372C(param_1 + 0x108,1,0x108,func_0889726C,0,0);
    func_0889726C(param_1,0);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
