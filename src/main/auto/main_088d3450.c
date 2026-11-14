/* func_088D3450: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0887B778(int param_1,int *param_2);
extern void func_0887B820(int param_1,int param_2);
extern void func_088DA0A8(float *param_1,int param_2,float *param_3,unsigned char *param_4);

void func_088D3450(int *param_1,int *param_2,int param_3)

{
  char auStack_50 [64];
  int local_10;
  
  if ((*param_1 != 0) && (*param_2 != 0)) {
    func_088DA0A8(*param_2,param_3,auStack_50,&local_10);
    func_0887B778(*param_1,auStack_50);
    func_0887B820(*param_1,local_10);
  }
  return;
}
