/* func_088D7F70: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D8000(char *param_1);
extern int D_089CC188;

int func_088D7F70(int param_1)

{
  *(char **)(param_1 + 0x24) = &D_089CC188;
  func_088D8000(param_1);
  return param_1;
}
