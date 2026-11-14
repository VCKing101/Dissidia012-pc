/* func_088B3868: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088B38E0(int *param_1);
extern void func_08992F04(int param_1);
extern int D_089CB7C8;

void func_088B3868(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x1c) = &D_089CB7C8;
    func_088B38E0(param_1);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
