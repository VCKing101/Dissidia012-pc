/* func_088C2C5C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088ABA9C(int param_1);
extern int D_089D712C;

unsigned char func_088C2C5C(int *param_1,int param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = func_088ABA9C(D_089D712C);
  *param_1 = iVar1 / 0xe10;
  iVar2 = *param_3;
  param_1[1] = iVar2;
  return iVar2 <= iVar1 / 0xe10;
}
