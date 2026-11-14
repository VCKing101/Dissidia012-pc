/* func_0895B1E4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0895B3CC();
extern void func_0895B43C();
extern int D_089DBF40;
extern int D_09B6F478;

void func_0895B1E4(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0xc) = &D_089DBF40;
    func_0895B3CC(param_1,D_09B6F478);
    if ((param_2 & 1) != 0) {
      func_0895B43C(param_1);
    }
  }
  return;
}
