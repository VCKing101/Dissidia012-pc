/* func_088A4C10: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
typedef int code();

void func_088A4C10(int param_1,int param_2,int param_3)

{
  if (*(code **)(param_1 + 0x38) != (code *)0x0) {
    (**(code **)(param_1 + 0x38))(param_2,param_3,*(int *)(param_1 + 0x3c));
  }
  return;
}
