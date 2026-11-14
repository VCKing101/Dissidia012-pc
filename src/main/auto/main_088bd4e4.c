/* func_088BD4E4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

int * func_088BD4E4(int *param_1)

{
  int *puVar1;
  int iVar2;
  
  *param_1 = 0;
  param_1[1] = 0;
  iVar2 = 0;
  puVar1 = param_1;
  do {
    puVar1[2] = 0;
    iVar2 = iVar2 + 1;
    puVar1 = puVar1 + 1;
  } while (iVar2 < 0xc);
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  return param_1;
}
