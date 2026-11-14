/* func_088D4584: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0887B778(int param_1,int *param_2);
extern void func_0887B828(int param_1,int param_2);
extern void func_088DA0A8(float *param_1,int param_2,float *param_3,unsigned char *param_4);

void func_088D4584(int param_1,int param_2,int param_3,int param_4)

{
  char auStack_50 [64];
  char auStack_10 [4];
  
  func_088DA0A8(*(int *)(param_1 + param_2 * 4),param_3,auStack_50,auStack_10);
  func_0887B778(param_4,auStack_50);
  func_0887B828(param_4,auStack_10);
  return;
}
