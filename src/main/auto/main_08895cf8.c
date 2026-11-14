/* func_08895CF8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088A9E60();
extern int D_08895DC0;

void func_08895CF8(int param_1,int param_2)

{
  char auStack_90 [128];
  
  if (*(int *)(param_1 + 8) != 0) {
    func_088A9E60(*(int *)(param_1 + 8),auStack_90,0x20,&D_08895DC0,param_2,0,0,0);
  }
  return;
}
