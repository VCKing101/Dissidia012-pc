/* func_088C2604: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088CC440(int param_1);

int
func_088C2604(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int uVar1;
  
  uVar1 = *(int *)(param_5 + 4);
  func_088CC440(uVar1);
  *(int *)(param_2 + 4) = uVar1;
  return 1;
}
