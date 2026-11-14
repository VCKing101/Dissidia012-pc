/* func_0896BD58: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern char * func_08935828();
extern int * func_08969F74();
extern int D_089DC548;

int func_0896BD58(int param_1)

{
  func_08969F74();
  *(char **)(param_1 + 0x20) = &D_089DC548;
  *(int *)(param_1 + 0x24) = 0;
  func_08935828(param_1 + 0x28,0,0x10);
  func_08935828(param_1 + 0x38,0,0x10);
  *(int *)(param_1 + 0x48) = 0;
  *(char *)(param_1 + 0x4c) = 1;
  *(char *)(param_1 + 0x4d) = 0;
  return param_1;
}
