/* func_0896D0D0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0896CD88(int param_1,float *param_2,float *param_3);

int func_0896D0D0(int param_1,int param_2,int param_3)

{
  func_0896CD88(param_2,param_3,
               *(int *)(param_2 + 0x8c) +
               (unsigned int)*(unsigned short *)
                      (*(int *)(*(int *)(param_2 + 0x90) + 8) +
                      *(int *)(*(int *)(param_2 + 0x90) + 4) * 2) + 0x60);
  return 1;
}
