/* func_088D3AE8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D9AF4(int param_1,int param_2);
extern void func_088D9B84(int param_1,float *param_2);
extern void func_088DA0A8(float *param_1,int param_2,float *param_3,unsigned char *param_4);

void func_088D3AE8(int param_1,int param_2,int param_3,int param_4)

{
  int *puVar1;
  char auStack_60 [64];
  char auStack_20 [4];
  
  func_088DA0A8(*(int *)(param_1 + param_2 * 4),param_3,auStack_60,auStack_20);
  puVar1 = (int *)(param_1 + param_4 * 4);
  func_088D9B84(*puVar1,auStack_60);
  func_088D9AF4(*puVar1,auStack_20);
  return;
}
