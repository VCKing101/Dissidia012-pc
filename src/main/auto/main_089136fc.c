/* func_089136FC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088CD624();
extern int func_088CE430();
extern int func_088D6E24();
extern int func_088DE400();
extern void func_0892F4EC();

void func_089136FC(int param_1)

{
  func_088CD624(param_1 + 0x3c0,*(int *)(param_1 + 4),3,0x5a,0x24,0x8a02,0x8801,1);
  func_088D6E24(param_1 + 0xa0,*(int *)(param_1 + 4),3,100,0x24,0x8a02,0x8801,param_1 + 0x3c0,
               0,0);
  func_088DE400(param_1 + 0x31c,*(int *)(param_1 + 4),3,100,0x24,0x8a04,0x8801,0);
  func_0892F4EC(param_1 + 0x418,*(int *)(param_1 + 4),3,100,0x24,0x8a26,0x8802,0x8a28,0x8828);
  func_088CE430(param_1 + 0x404,*(int *)(param_1 + 4),3,100,0x24,0x8a02,0x8801);
  *(char *)(param_1 + 0x7d7) = 1;
  return;
}
