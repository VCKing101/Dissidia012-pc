/* func_089726F8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08972964(int param_1,int param_2);

void func_089726F8(int param_1,int param_2)

{
  if (*(int *)(*(int *)(param_1 + 4) + param_2 * 4) != 0) {
    func_08972964(param_1,param_2);
    *(int *)(*(int *)(param_1 + 4) + param_2 * 4) = 0;
  }
  return;
}
