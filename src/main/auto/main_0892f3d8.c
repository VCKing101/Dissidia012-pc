/* func_0892F3D8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0892ABF8(int param_1,unsigned int param_2);
extern void func_08992F04(int param_1);
extern int D_089CF010;

void func_0892F3D8(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x24) = &D_089CF010;
    func_0892ABF8(param_1,0);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
