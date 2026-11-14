/* func_088D89B8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0887A2EC(int param_1,unsigned int param_2);
extern void func_088D8A54(int param_1);
extern void func_08992F04(int param_1);
extern int D_089CC1D8;

void func_088D89B8(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x40) = &D_089CC1D8;
    func_088D8A54(param_1);
    func_0887A2EC(param_1 + 8,2);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
