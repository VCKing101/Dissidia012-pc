/* func_0888E5D0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int * func_088699F4();
extern void func_08895F18();
extern void func_08896AB0();
extern void func_088A0D1C();
extern int func_088A0DFC();
extern int D_089D6A90;
extern int D_089D6A94;
extern int D_089D6AE8;
extern int D_089D6AF0;
extern int D_089D6C80;
extern int D_09B491E0;

void func_0888E5D0(void)

{
  if (D_089D6A94 != -1) {
    func_088A0DFC(D_089D6C80,D_089D6A94);
    D_089D6A94 = -1;
  }
  func_08896AB0(D_089D6AF0);
  func_08895F18(D_089D6AE8);
  func_088699F4(&D_09B491E0);
  if (D_089D6A90 != 0) {
    func_088A0D1C();
  }
  return;
}
