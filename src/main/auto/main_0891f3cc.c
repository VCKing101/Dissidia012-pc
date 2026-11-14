/* func_0891F3CC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0891F0A0();
extern void func_0891F0BC();
extern int func_089209F4();
extern void func_08993560();
extern int D_08922890;
extern int D_08924F60;

int func_0891F3CC(int param_1)

{
  func_089209F4();
  func_08993560(param_1 + 0x10,0x46,8,&D_08924F60);
  func_08993560(param_1 + 0x240,0x20,0x24,&D_08922890);
  func_08993560(param_1 + 0x6c0,6,0x14,func_0891F0A0);
  func_0891F0BC(param_1);
  return param_1;
}
