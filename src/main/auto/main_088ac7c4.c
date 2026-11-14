/* func_088AC7C4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088AC82C(int param_1,char param_2);

void func_088AC7C4(int param_1)

{
  *(unsigned int *)(param_1 + 0xef8) = *(unsigned int *)(param_1 + 0xef8) | 0x80;
  func_088AC82C(param_1,0);
  return;
}
