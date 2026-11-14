/* func_088D7B3C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D7BCC(char *param_1);
extern int D_089CC0C8;

int func_088D7B3C(int param_1)

{
  *(char **)(param_1 + 0x1c) = &D_089CC0C8;
  func_088D7BCC(param_1);
  return param_1;
}
