/* func_088C483C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088CC22C(void);
extern int func_088CC2B0(void);

unsigned char func_088C483C(int *param_1)

{
  int iVar1;
  
  iVar1 = func_088CC2B0();
  *param_1 = iVar1;
  iVar1 = func_088CC22C();
  param_1[1] = iVar1;
  return iVar1 <= *param_1;
}
