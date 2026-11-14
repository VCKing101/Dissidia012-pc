/* func_088C2640: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_089200F0(int *param_1,int param_2);
extern int D_089D712C;

int
func_088C2640(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int uVar1;
  
  uVar1 = *(int *)(param_5 + 4);
  func_089200F0(D_089D712C + 0x3f548,uVar1);
  *(int *)(param_2 + 4) = uVar1;
  return 1;
}
