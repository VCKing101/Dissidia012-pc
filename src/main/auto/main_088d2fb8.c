/* func_088D2FB8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D9B48(int param_1,int param_2);
extern void func_088D9B84(int param_1,float *param_2);
extern void func_088DA0A8(float *param_1,int param_2,float *param_3,unsigned char *param_4);

void func_088D2FB8(int *param_1,int *param_2,int param_3,unsigned int param_4)

{
  char auStack_50 [64];
  int local_10;
  
  if ((*param_1 != 0) && (*param_2 != 0)) {
    func_088DA0A8(*param_2,param_3,auStack_50,&local_10);
    if ((param_4 & 1) != 0) {
      func_088D9B84(*param_1,auStack_50);
    }
    if ((param_4 & 2) != 0) {
      func_088D9B48(*param_1,local_10);
    }
  }
  return;
}
