/* func_08943084: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0894344C();
extern void func_08970F00();
extern void func_089748D0();

void func_08943084(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    func_0894344C(param_1);
    *(int *)(param_1 + 0x9c) = 0;
    *(int *)(param_1 + 0xa0) = 0;
    *(int *)(param_1 + 0xa4) = 0;
    *(int *)(param_1 + 0xa8) = 0;
    *(int *)(param_1 + 0xac) = 0;
    *(int *)(param_1 + 0xb0) = 0;
    func_08970F00(param_1 + 0x28,2);
    if ((param_2 & 1) != 0) {
      func_089748D0(param_1);
    }
  }
  return;
}
