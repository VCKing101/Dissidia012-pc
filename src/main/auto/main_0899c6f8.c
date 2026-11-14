/* func_0899C6F8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile). Names are placeholders. */
extern void func_0899B830(int param_1);
extern void func_0899B85C(int param_1);

void func_0899C6F8(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    if ((param_2 & 2) != 0) {
      if (*(int *)(param_1 + 0x18) != 0) {
        param_2 = param_2 | 1;
      }
      func_0899B85C(*(int *)(param_1 + 0x18));
      *(int *)(param_1 + 0x18) = 0;
    }
    if ((param_2 & 1) != 0) {
      func_0899B830(*(int *)(param_1 + 0x14));
      *(int *)(param_1 + 0x14) = 0;
    }
  }
  return;
}
