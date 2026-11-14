/* func_08932178: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D3F00(int param_1,int param_2,short *param_3);
extern void func_088D9AF4(int param_1,int param_2);
extern void func_088D9B84(int param_1,float *param_2);
extern void func_088DA0A8(float *param_1,int param_2,float *param_3,unsigned char *param_4);
extern int D_089CF2D8;

void func_08932178(int param_1)

{
  char auStack_50 [64];
  char auStack_10 [4];
  
  func_088DA0A8(*(int *)(param_1 + 4),*(int *)(param_1 + 8),auStack_50,auStack_10);
  func_088D9B84(*(int *)(param_1 + 0x10),auStack_50);
  func_088D9AF4(*(int *)(param_1 + 0x10),auStack_10);
  func_088D3F00(0,param_1 + 0x10,&D_089CF2D8);
  return;
}
