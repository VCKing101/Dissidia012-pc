/* func_088D91D0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D89B8();
extern void func_088D9254();
extern void func_08992F04();
extern void func_0899372C();
extern int D_089CC240;

void func_088D91D0(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0x1e4) = &D_089CC240;
    func_088D9254(param_1);
    func_0899372C(param_1 + 4,7,0x44,func_088D89B8,0,0);
    if ((param_2 & 1) != 0) {
      func_08992F04(param_1);
    }
  }
  return;
}
