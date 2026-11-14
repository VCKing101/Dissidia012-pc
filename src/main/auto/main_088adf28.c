/* func_088ADF28: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern char * func_0887AFBC(int *param_1,int param_2,char param_3);
extern char * func_08935BB4(char *param_1,char *param_2,int param_3);

void func_088ADF28(int param_1,int param_2,int param_3)

{
  int uVar1;
  
  uVar1 = func_0887AFBC(*(int *)(param_1 + 0xee0),param_3,0);
  func_08935BB4(param_2,uVar1,0x80);
  return;
}
