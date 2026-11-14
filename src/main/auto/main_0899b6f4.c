/* func_0899B6F4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile). Names are placeholders. */
typedef int code();
extern void func_08934AF4(int param_1);
extern void func_08934B18(int param_1);
extern int D_089DA4C4;
extern int D_089DA4D0;

void func_0899B6F4(code *param_1,code *param_2)

{
  if (param_1 == (code *)0x0) {
    param_1 = func_08934AF4;
  }
  if (param_2 == (code *)0x0) {
    param_2 = func_08934B18;
  }
  D_089DA4C4 = param_1;
  D_089DA4D0 = param_2;
  return;
}
