/* func_08932F14: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D3F00(int param_1,int param_2,short *param_3);
extern void func_08931E18(int *param_1);
extern int D_089CF340;

void func_08932F14(int param_1)

{
  if (*(int *)(param_1 + 0x28) == 1) {
    func_088D3F00(0,param_1 + 0x34,&D_089CF340);
    func_08931E18(param_1 + 0x38);
    func_08931E18(param_1 + 0x5c);
  }
  return;
}
