/* func_088A5884: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int * func_088698FC();
extern int func_088A9974();

int func_088A5884(int param_1)

{
  func_088A9974();
  func_088698FC(param_1 + 0x5c);
  *(char *)(param_1 + 0x38) = 0;
  *(int *)(param_1 + 0x58) = 0;
  *(int *)(param_1 + 0x60) = 0;
  *(char *)(param_1 + 100) = 0;
  return param_1;
}
