/* func_088D16A4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088A0DFC(int param_1,int param_2);
extern int D_089D6C80;

void func_088D16A4(int param_1)

{
  if (*(int *)(param_1 + 0x1c) != -1) {
    func_088A0DFC(D_089D6C80,*(int *)(param_1 + 0x1c));
    *(int *)(param_1 + 0x1c) = 0xffffffff;
  }
  return;
}
