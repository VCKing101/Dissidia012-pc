/* func_0893F058: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern unsigned int func_0893431C(int param_1);
extern int D_09B6F468;

void func_0893F058(int *param_1,int param_2)

{
  int iVar1;
  
  D_09B6F468 = 0;
  iVar1 = func_0893431C(param_2);
  if ((iVar1 == -1) && (D_09B6F468 != 0)) {
    *param_1 = D_09B6F468;
  }
  return;
}
