/* func_08925DC0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_08925DC0(int param_1,short param_2,short param_3,int param_4)

{
  *(short *)(*(int *)(param_1 + 0x20) + *(char *)(*(int *)(param_1 + 0x20) + 0xb7) * 8 + 0xbc)
       = param_2;
  *(short *)(*(int *)(param_1 + 0x20) + *(char *)(*(int *)(param_1 + 0x20) + 0xb7) * 8 + 0xbe)
       = param_3;
  *(int *)(*(int *)(param_1 + 0x20) + *(char *)(*(int *)(param_1 + 0x20) + 0xb7) * 8 + 0xc0)
       = param_4;
  *(char *)(*(int *)(param_1 + 0x20) + 0xb7) = *(char *)(*(int *)(param_1 + 0x20) + 0xb7) + '\x01';
  return;
}
