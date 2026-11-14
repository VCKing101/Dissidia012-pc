/* func_088D6C9C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D6D90(char *param_1);
extern int func_088D7B3C(int param_1);
extern int func_088D7F70(int param_1);
extern int func_088D8504(int param_1);
extern int func_088D9180(int param_1);
extern int func_088D96AC(int param_1);
extern int D_089CC0B0;

int func_088D6C9C(int param_1)

{
  *(char **)(param_1 + 0x278) = &D_089CC0B0;
  func_088D7F70(param_1 + 0x24);
  func_088D8504(param_1 + 0x4c);
  func_088D9180(param_1 + 0x5c);
  func_088D7B3C(param_1 + 0x244);
  func_088D96AC(param_1 + 0x264);
  func_088D6D90(param_1);
  return param_1;
}
