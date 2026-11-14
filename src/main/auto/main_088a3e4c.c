/* func_088A3E4C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
typedef int code();
extern unsigned int func_0889F704(int *param_1);

void func_088A3E4C(int param_1)

{
  if (*(code **)(param_1 + 0x28) != (code *)0x0) {
    (**(code **)(param_1 + 0x28))(*(unsigned int *)(param_1 + 0x2c));
  }
  if (*(unsigned char *)(param_1 + 0x31) == '\0') {
    func_0889F704(param_1 + 4);
    *(unsigned char *)(param_1 + 0x30) = 0;
  }
  return;
}
