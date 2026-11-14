/* func_088D4908: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D4744();

void func_088D4908(int param_1,int param_2,int param_3)

{
  *(int *)(param_1 + 8) = param_2;
  *(int *)(param_1 + 0xc) = param_3;
  func_088D4744();
  return;
}
