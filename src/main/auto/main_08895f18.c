/* func_08895F18: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0889F704(int *param_1);

void func_08895F18(int *param_1)

{
  if (param_1[0xe] != 0) {
    func_0889F704(param_1 + 5);
    param_1[0xe] = 0;
  }
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  return;
}
