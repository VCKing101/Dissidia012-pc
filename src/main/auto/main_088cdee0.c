/* func_088CDEE0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088CDF70(char *param_1);
extern int D_089CBD50;

int func_088CDEE0(int param_1)

{
  *(char **)(param_1 + 0xc) = &D_089CBD50;
  func_088CDF70(param_1);
  return param_1;
}
