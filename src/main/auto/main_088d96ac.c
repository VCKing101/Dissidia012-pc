/* func_088D96AC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D973C(char *param_1);
extern int D_089CC250;

int func_088D96AC(int param_1)

{
  *(char **)(param_1 + 0x10) = &D_089CC250;
  func_088D973C(param_1);
  return param_1;
}
