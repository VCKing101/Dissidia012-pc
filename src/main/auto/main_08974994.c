/* func_08974994: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern char * func_08935828(char *param_1,char param_2,int param_3);

int func_08974994(int param_1)

{
  *(int *)(param_1 + 8) = 0;
  *(int *)(param_1 + 0xc) = 0;
  *(int *)(param_1 + 0x10) = 0;
  *(int *)(param_1 + 0x14) = 0;
  func_08935828(param_1,0,8);
  return param_1;
}
