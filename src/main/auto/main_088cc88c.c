/* func_088CC88C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0887A644(int param_1,unsigned int param_2);
extern void func_088CC914(int param_1);
extern void func_088CD280(int param_1);
extern void func_08992F04(int param_1);
extern int D_089CBCE0;

void func_088CC88C(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x34) = &D_089CBCE0;
    func_088CC914(param_1);
    func_088CD280(param_1);
    func_0887A644(param_1 + 0x24,2);
    func_0887A644(param_1 + 0x14,2);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
