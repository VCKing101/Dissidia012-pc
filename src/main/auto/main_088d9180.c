/* func_088D9180: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088D8978();
extern void func_088D9248();
extern void func_08993560();
extern int D_089CC240;

int func_088D9180(int param_1)

{
  *(char **)(param_1 + 0x1e4) = &D_089CC240;
  func_08993560(param_1 + 4,7,0x44,func_088D8978);
  func_088D9248(param_1);
  return param_1;
}
