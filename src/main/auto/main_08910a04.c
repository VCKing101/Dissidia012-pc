/* func_08910A04: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int * func_088699F4();
extern int func_0888E4A0();
extern int func_0888E5D0();
extern int func_08895AE4();
extern int func_08895B84();
extern int func_08896AB0();
extern int func_088A0CD4();
extern int func_088A0D1C();
extern int func_08AA7850();
extern int func_08ACCE68();
extern int D_089D6AE0;
extern int D_089D6AF0;
extern int D_08AAAD80;
extern int D_08ACD0E8;

void func_08910A04(int param_1)

{
  func_08895B84(D_089D6AE0);
  func_08895AE4(D_089D6AE0);
  func_0888E5D0();
  func_0888E4A0();
  func_08896AB0(D_089D6AF0);
  func_088699F4(param_1);
  if (*(int *)(param_1 + 4) != 0) {
    func_088A0D1C();
    func_088A0CD4(*(int *)(param_1 + 4));
    *(int *)(param_1 + 4) = 0;
  }
  func_08AA7850(D_08AAAD80);
  func_08ACCE68(D_08ACD0E8);
  return;
}
