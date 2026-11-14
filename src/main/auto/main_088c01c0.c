/* func_088C01C0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern char * func_08935828(char *param_1,char param_2,int param_3);

void func_088C01C0(int param_1)

{
  func_08935828(param_1,0,8);
  func_08935828(param_1 + 8,0,4);
  func_08935828(param_1 + 0xc,0,0x100);
  func_08935828(param_1 + 0x10c,0,4);
  *(short *)(param_1 + 0x110) = 0;
  *(short *)(param_1 + 0x112) = 0;
  *(int *)(param_1 + 0x114) = 0;
  return;
}
