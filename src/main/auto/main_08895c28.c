/* func_08895C28: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088AA130(int param_1,int param_2,int param_3);
extern int D_08895E00;

void func_08895C28(int param_1)

{
  if (*(int *)(param_1 + 8) != 0) {
    func_088AA130(*(int *)(param_1 + 8),&D_08895E00,0);
  }
  return;
}
