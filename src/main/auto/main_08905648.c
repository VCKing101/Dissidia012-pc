/* func_08905648: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_089C59C0();

int func_08905648(int *param_1)

{
  int iVar1;
  
  if (*param_1 != -1) {
    iVar1 = func_089C59C0(*param_1,0);
    *param_1 = -1;
    if (iVar1 < 0) {
      return 0;
    }
  }
  return 1;
}
