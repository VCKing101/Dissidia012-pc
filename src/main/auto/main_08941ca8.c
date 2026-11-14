/* func_08941CA8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08941D14();
extern int D_089DAFC0;

int func_08941CA8(int param_1)

{
  *(char **)(param_1 + 0x24) = &D_089DAFC0;
  func_08941D14(param_1);
  return param_1;
}
