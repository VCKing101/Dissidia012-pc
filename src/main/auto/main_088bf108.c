/* func_088BF108: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int * func_088BD4E4();
extern int func_088BF08C();
extern void func_08993560();

int func_088BF108(int param_1)

{
  func_088BD4E4();
  func_088BD4E4(param_1 + 0x40);
  func_08993560(param_1 + 0x80,10,0x44,func_088BF08C);
  func_08993560(param_1 + 0x32c,10,0x44,func_088BF08C);
  *(int *)(param_1 + 0x5d8) = 0;
  *(int *)(param_1 + 0x5dc) = 0;
  *(int *)(param_1 + 0x5e0) = 0;
  return param_1;
}
