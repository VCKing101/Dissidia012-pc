/* func_088AC7EC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088AC07C(int param_1,char param_2);

void func_088AC7EC(int param_1)

{
  *(int *)(param_1 + 0x210) = 7;
  *(int *)(param_1 + 0x7a8) = 1;
  *(char *)(param_1 + 0x761) = 0;
  *(unsigned int *)(param_1 + 0xef8) = *(unsigned int *)(param_1 + 0xef8) & 0xfffffffd;
  func_088AC07C(param_1,0);
  return;
}
