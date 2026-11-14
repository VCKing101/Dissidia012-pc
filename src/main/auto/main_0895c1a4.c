/* func_0895C1A4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0895B1E4(int param_1,unsigned int param_2);
extern void func_0895C31C(int param_1);
extern int D_089DBF60;

void func_0895C1A4(int param_1,unsigned int param_2)

{
  if (param_1 != 0) {
    *(char **)(param_1 + 0xc) = &D_089DBF60;
    func_0895B1E4(param_1,0);
    if ((param_2 & 1) != 0) {
      func_0895C31C(param_1);
    }
  }
  return;
}
