/* func_08910904: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088CAF90(int *param_1);
extern int func_088CB054(int param_1);
extern void func_089110FC(int param_1);

void func_08910904(int *param_1)

{
  func_088CB054(param_1[1]);
  func_088CAF90(param_1[1]);
  func_089110FC(param_1[1]);
  param_1[1] = 0;
  func_089110FC(*param_1);
  *param_1 = 0;
  return;
}
