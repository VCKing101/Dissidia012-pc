/* func_088AEFA0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern char * func_08935828(char *param_1,char param_2,int param_3);

void func_088AEFA0(int param_1)

{
  *(int *)(*(int *)(param_1 + 0xc) + 0x2b30) = 0;
  *(int *)(*(int *)(param_1 + 0xc) + 0x2b34) = 0;
  *(int *)(*(int *)(param_1 + 0xc) + 0x2b38) = 0;
  func_08935828(*(int *)(param_1 + 0xc) + 0xe0c,0,0x40);
  func_08935828(*(int *)(param_1 + 0xc) + 0x2a94,0,0x14);
  **(int **)(param_1 + 8) = *(int *)(*(int *)(param_1 + 0xc) + 0x2b84);
  *(int *)(*(int *)(param_1 + 8) + 4) = *(int *)(*(int *)(param_1 + 0xc) + 0x2b88);
  return;
}
