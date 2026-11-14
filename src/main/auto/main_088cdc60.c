/* func_088CDC60: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0887AD1C(int *param_1);
extern unsigned char func_0887AFB0(int param_1);
extern void func_088CDC58(int param_1);

void func_088CDC60(int param_1)

{
  int iVar1;
  
  iVar1 = func_0887AFB0(param_1 + 0x2c);
  if (iVar1 == 1) {
    func_0887AD1C(param_1 + 0x2c);
  }
  func_088CDC58(param_1);
  return;
}
