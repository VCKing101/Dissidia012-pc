/* func_088A193C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int * func_088A077C();
extern char * func_08935828();
extern void func_08993560();
extern int D_088A1908;
extern int D_088A1930;

int * func_088A193C(int *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  func_088A077C(param_1 + 5);
  func_08993560(param_1 + 0x1a,0x14,1,&D_088A1930);
  func_08993560(param_1 + 0x1f,3,0x10,&D_088A1908);
  func_08935828(param_1 + 2,0,0xc);
  return param_1;
}
