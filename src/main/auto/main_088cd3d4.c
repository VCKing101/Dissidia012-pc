/* func_088CD3D4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0887A2B4(int param_1);
extern int func_0887A61C(int param_1);
extern void func_088CD49C(char *param_1);
extern void func_088CDC58(int param_1);
extern int D_089CBD10;

int func_088CD3D4(int param_1)

{
  *(char **)(param_1 + 0x40) = &D_089CBD10;
  func_0887A2B4(param_1 + 0xc);
  func_0887A61C(param_1 + 0x2c);
  func_088CD49C(param_1);
  func_088CDC58(param_1);
  return param_1;
}
