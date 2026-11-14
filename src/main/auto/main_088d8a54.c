/* func_088D8A54: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D8A1C();
extern void func_088D8A98();
extern void func_088DA060();

void func_088D8A54(int param_1)

{
  func_088D8A98();
  if (*(int *)(param_1 + 4) != 0) {
    func_088DA060();
    *(int *)(param_1 + 4) = 0;
  }
  func_088D8A1C(param_1);
  return;
}
