/* func_088A3024: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
typedef int code();

void func_088A3024(int param_1)

{
  *(unsigned int *)(param_1 + 0x34) = *(unsigned int *)(param_1 + 0x34) | 2;
  if (*(code **)(param_1 + 0x44) != (code *)0x0) {
    (**(code **)(param_1 + 0x44))(param_1,*(int *)(param_1 + 0x48));
  }
  return;
}
