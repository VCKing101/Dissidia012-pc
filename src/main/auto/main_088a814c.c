/* func_088A814C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088A9880(int param_1,unsigned int param_2);
extern void func_08992F04(int param_1);
extern int D_089CB1B0;

void func_088A814C(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x2c) = &D_089CB1B0;
    func_088A9880(param_1,0);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
