/* func_08894E28: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08864C48(int param_1,unsigned int param_2);
extern void func_0886EE90(int param_1,unsigned int param_2);
extern void func_0886F048(int param_1,unsigned int param_2);
extern void func_08992F04(int param_1);
extern int D_089CA868;

void func_08894E28(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x120) = &D_089CA868;
    func_08864C48(param_1 + 0x78,2);
    func_0886F048(param_1 + 0x60,2);
    func_0886EE90(param_1 + 0x10,2);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
