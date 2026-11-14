/* func_088CC83C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0887A61C(int param_1);
extern void func_088CC904(char *param_1);
extern void func_088CD278(int param_1);
extern int D_089CBCE0;

int func_088CC83C(int param_1)

{
  *(char **)(param_1 + 0x34) = &D_089CBCE0;
  func_0887A61C(param_1 + 0x14);
  func_0887A61C(param_1 + 0x24);
  func_088CC904(param_1);
  func_088CD278(param_1);
  return param_1;
}
