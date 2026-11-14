/* func_088D8978: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0887A2B4(int param_1);
extern void func_088D8A1C(char *param_1);
extern int D_089CC1D8;

int func_088D8978(int param_1)

{
  *(char **)(param_1 + 0x40) = &D_089CC1D8;
  func_0887A2B4(param_1 + 8);
  func_088D8A1C(param_1);
  return param_1;
}
