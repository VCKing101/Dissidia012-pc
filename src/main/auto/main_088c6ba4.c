/* func_088C6BA4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0887A644(int param_1,unsigned int param_2);
extern void func_088C6C24(int *param_1);
extern void func_08992F04(int param_1);
extern int D_089CBC38;

void func_088C6BA4(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x28) = &D_089CBC38;
    func_088C6C24(param_1);
    func_0887A644(param_1 + 0x18,2);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
