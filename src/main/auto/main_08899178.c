/* func_08899178: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern unsigned char func_08893CF8(int param_1);
extern int D_089D6AC8;

void func_08899178(int param_1,int param_2,int param_3)

{
  func_08893CF8(D_089D6AC8);
  *(int *)(param_1 + 0xf4) = param_2;
  *(int *)(param_1 + 0xf8) = param_3;
  *(int *)(param_1 + 0xfc) = 0;
  *(int *)(param_1 + 0x100) = 1;
  return;
}
