/* func_0892B590: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D9CFC(int param_1);
extern void func_088D9F5C(int param_1,int param_2,char param_3);

void func_0892B590(int param_1)

{
  if (*(int *)(param_1 + 0x10) != 0) {
    func_088D9F5C(*(int *)(param_1 + 0x10),0,0);
  }
  if (*(int *)(param_1 + 0x14) != 0) {
    func_088D9CFC(*(int *)(param_1 + 0x14));
  }
  return;
}
