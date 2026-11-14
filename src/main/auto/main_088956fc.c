/* func_088956FC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08895448(int param_1,unsigned int param_2);
extern void func_088954FC(int param_1);
extern int D_089CA8B0;

void func_088956FC(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 8) = &D_089CA8B0;
    func_08895448(param_1,0);
    if ((param_2 & 1) != 0) {
      func_088954FC(param_1);
    }
  }
  return;
}
