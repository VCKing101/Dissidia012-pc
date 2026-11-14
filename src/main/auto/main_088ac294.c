/* func_088AC294: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088AC034(int param_1,int param_2);
extern int func_088AC07C(int param_1,char param_2);

void func_088AC294(int param_1,int param_2)

{
  *(int *)(param_1 + 0x210) = param_2;
  *(int *)(param_1 + 0x7a8) = 3;
  *(char *)(param_1 + 0x761) = 0;
  if (param_2 == 4) {
    *(unsigned int *)(param_1 + 0xef8) = *(unsigned int *)(param_1 + 0xef8) & 0xfffffffd;
  }
  func_088AC034(param_1,param_2);
  func_088AC07C(param_1,1);
  return;
}
