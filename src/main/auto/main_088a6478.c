/* func_088A6478: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088A4C10(int param_1,int param_2,int param_3);

void func_088A6478(int *param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  
  iVar1 = 0;
  while( 1 ) {
    if (iVar1 == 0) {
      iVar1 = *param_1;
    }
    else {
      iVar1 = *(int *)(iVar1 + 0x40);
    }
    if (iVar1 == 0) break;
    if (param_2 == *(int *)(iVar1 + 0x10)) {
      func_088A4C10(iVar1,param_3,param_4);
      return;
    }
  }
  return;
}
