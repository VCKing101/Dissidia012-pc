/* func_088C6C24: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0887AD1C();
extern void func_08882158();
extern void func_088C6C08();
extern void func_08992F04();

void func_088C6C24(int *param_1)

{
  func_0887AD1C(param_1 + 6);
  if (param_1[5] != 0) {
    func_08992F04();
  }
  if (*param_1 != 0) {
    func_08882158();
  }
  func_088C6C08(param_1);
  return;
}
