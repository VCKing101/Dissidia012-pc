/* func_088CD424: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0887A2EC(int param_1,unsigned int param_2);
extern void func_0887A644(int param_1,unsigned int param_2);
extern void func_088CD4C8(int param_1);
extern void func_088CDC60(int param_1);
extern void func_08992F04(int param_1);
extern int D_089CBD10;

void func_088CD424(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x40) = &D_089CBD10;
    func_088CD4C8(param_1);
    func_088CDC60(param_1);
    func_0887A644(param_1 + 0x2c,2);
    func_0887A2EC(param_1 + 0xc,2);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
