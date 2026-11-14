/* func_088D53BC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0889F290();
extern unsigned char func_0889F2D4();
extern int D_089D6C30;

void func_088D53BC(int *param_1)

{
  if ((param_1[2] != 0) && (param_1[3] != 0)) {
    func_0889F290(D_089D6C30,param_1[2],param_1[3]);
    func_0889F2D4(D_089D6C30,*param_1);
  }
  return;
}
