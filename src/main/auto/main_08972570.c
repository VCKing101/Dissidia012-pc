/* func_08972570: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_089748A4();
extern int func_0899350C();
extern int func_089C2D10();
extern int D_089C2CD8;

int func_08972570(int *param_1,int param_2)

{
  int uVar1;
  
  *param_1 = param_2;
  uVar1 = func_0899350C(param_2,4,&D_089C2CD8,0,func_089C2D10,0,0);
  param_1[1] = uVar1;
  uVar1 = func_089748A4(param_2);
  param_1[2] = uVar1;
  return 1;
}
