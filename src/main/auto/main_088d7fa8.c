/* func_088D7FA8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D8030();
extern void func_08992F04();
extern int D_089CC188;

void func_088D7FA8(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x24) = &D_089CC188;
    func_088D8030(param_1);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
