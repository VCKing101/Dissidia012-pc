/* func_088AF75C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern char * func_08935BB4(char *param_1,char *param_2,int param_3);

void func_088AF75C(int param_1,int param_2)

{
  *(int *)(*(int *)(param_1 + 0xc) + 0x814) = 0xb;
  *(int *)(*(int *)(param_1 + 0xc) + 0x2b3c) = param_2;
  func_08935BB4(*(int *)(param_1 + 0xc) + 0x830,*(int *)(param_1 + 0xc) + 0x2b40,0x14);
  *(int *)(*(int *)(param_1 + 0xc) + 0xdd4) = 1;
  return;
}
