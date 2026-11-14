/* func_0896E1E8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0896DEAC(int param_1,float *param_2,float *param_3);

int func_0896E1E8(int param_1,int param_2,int param_3)

{
  func_0896DEAC(param_2,param_3,
               *(int *)(param_2 + 0x8c) +
               (unsigned int)*(unsigned short *)
                      (*(int *)(*(int *)(param_2 + 0x90) + 8) +
                      *(int *)(*(int *)(param_2 + 0x90) + 4) * 2) + 0x60);
  return 1;
}
