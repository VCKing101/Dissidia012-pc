/* func_08897518: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0885C6CC(int *param_1,int param_2,int param_3,int param_4,int param_5,
                char param_6);
extern char * func_08935524(char *param_1,char *param_2,int param_3);

int func_08897518(int *param_1,int param_2,int param_3)

{
  int uVar1;
  
  uVar1 = func_0885C6CC(param_1[0x3c],param_3,0x40,0,1,0);
  *param_1 = uVar1;
  func_08935524(uVar1,param_2,param_3);
  return *param_1;
}
