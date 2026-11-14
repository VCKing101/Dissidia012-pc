/* func_088D8504: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D8594(char *param_1);
extern int D_089CC1C0;

int func_088D8504(int param_1)

{
  *(char **)(param_1 + 0xc) = &D_089CC1C0;
  func_088D8594(param_1);
  return param_1;
}
