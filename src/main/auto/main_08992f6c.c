/* func_08992F6C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08992F50(void);
extern int D_09AF6780;

void func_08992F6C(int *param_1)

{
  if ((*param_1 != 0) || (param_1 == D_09AF6780)) {
    func_08992F50();
  }
  *param_1 = (int)D_09AF6780;
  D_09AF6780 = param_1;
  return;
}
