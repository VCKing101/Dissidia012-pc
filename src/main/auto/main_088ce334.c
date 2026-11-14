/* func_088CE334: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088CE3C4(char *param_1);
extern int D_089CBD90;

int func_088CE334(int param_1)

{
  *(char **)(param_1 + 0x10) = &D_089CBD90;
  func_088CE3C4(param_1);
  return param_1;
}
