/* func_088C6FD4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088C6F68(short *param_1);

void func_088C6FD4(short *param_1)

{
  *param_1 = 0xffff;
  param_1[1] = 0xffff;
  param_1[2] = 0;
  param_1[3] = 0;
  func_088C6F68(param_1 + 4);
  func_088C6F68(param_1 + 0x22);
  return;
}
