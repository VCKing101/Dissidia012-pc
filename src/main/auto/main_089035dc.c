/* func_089035DC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_089035A4();
extern void func_0890418C();
extern int func_089057BC();
extern void func_08993560();
extern int D_089D8148;

int func_089035DC(int param_1)

{
  func_08993560(param_1 + 8,0x10,0x10,func_089035A4);
  func_089057BC(param_1 + 0x116);
  func_089057BC(param_1 + 0x11d);
  func_08993560(param_1 + 0x124,0x10,6,func_089057BC);
  D_089D8148 = param_1;
  func_0890418C(param_1);
  return param_1;
}
