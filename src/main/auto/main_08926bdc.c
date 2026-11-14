/* func_08926BDC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern char * func_08935524(char *param_1,char *param_2,int param_3);

int func_08926BDC(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(*(int *)(param_1 + 0x20) + 0xa20);
  func_08935524(*(int *)(param_1 + 0x20) + iVar1 * 0x40 + 0x1a24,param_2,0x40);
  *(int *)(*(int *)(param_1 + 0x20) + 0xa20) = *(int *)(*(int *)(param_1 + 0x20) + 0xa20) + 1;
  return iVar1;
}
