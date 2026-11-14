/* func_088D6DBC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D6D90();
extern void func_088D7BF0();
extern void func_088D8030();
extern void func_088D85A4();
extern void func_088D9254();
extern void func_088D9750();
extern void func_088DAB44();

void func_088D6DBC(int param_1)

{
  func_088D9750(param_1 + 0x264);
  func_088D7BF0(param_1 + 0x244);
  func_088D9254(param_1 + 0x5c);
  func_088D85A4(param_1 + 0x4c);
  func_088D8030(param_1 + 0x24);
  if (*(int *)(param_1 + 0xc) != 0) {
    func_088DAB44();
    *(int *)(param_1 + 0xc) = 0;
  }
  func_088D6D90(param_1);
  return;
}
