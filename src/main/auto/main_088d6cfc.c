/* func_088D6CFC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D6DBC(int param_1);
extern void func_088D7B74(int param_1,unsigned int param_2);
extern void func_088D7FA8(int param_1,unsigned int param_2);
extern void func_088D853C(int param_1,unsigned int param_2);
extern void func_088D91D0(int param_1,unsigned int param_2);
extern void func_088D96E4(int param_1,unsigned int param_2);
extern void func_08992F04(int param_1);
extern int D_089CC0B0;

void func_088D6CFC(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x278) = &D_089CC0B0;
    func_088D6DBC(param_1);
    func_088D96E4(param_1 + 0x264,2);
    func_088D7B74(param_1 + 0x244,2);
    func_088D91D0(param_1 + 0x5c,2);
    func_088D853C(param_1 + 0x4c,2);
    func_088D7FA8(param_1 + 0x24,2);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
