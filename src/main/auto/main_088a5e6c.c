/* func_088A5E6C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088A4D04();
extern void func_088A5830();
extern void func_088A5EBC();
extern void func_08940610();
extern void func_0894065C();
extern int D_089D6D78;

void func_088A5E6C(int param_1)

{
  func_088A5EBC();
  func_088A5830(D_089D6D78);
  func_088A4D04();
  func_0894065C(*(int *)(param_1 + 8));
  func_08940610(*(int *)(param_1 + 8),3);
  *(int *)(param_1 + 8) = 0;
  return;
}
