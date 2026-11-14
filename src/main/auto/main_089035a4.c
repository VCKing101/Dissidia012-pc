/* func_089035A4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_089057BC();
extern void func_08905848();

int func_089035A4(int param_1)

{
  func_089057BC();
  *(int *)(param_1 + 8) = 0;
  *(int *)(param_1 + 0xc) = 0;
  func_08905848(param_1);
  return param_1;
}
