/* func_08892234: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08894E28();
extern void func_08992F04();
extern void func_0899372C();
extern int D_089CA758;
extern int D_089CA768;

void func_08892234(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x734) = &D_089CA758;
    *(char **)(param_1 + 0x120) = &D_089CA768;
    func_08894E28(param_1 + 0x4e0,2);
    func_0899372C(param_1 + 0x140,3,0x130,func_08894E28,0,0);
    func_08894E28(param_1,0);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
