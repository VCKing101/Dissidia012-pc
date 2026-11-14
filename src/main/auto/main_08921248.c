/* func_08921248: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088470DC();

void func_08921248(int param_1,int param_2)

{
  short *psVar1;
  
  psVar1 = (short *)func_088470DC(param_2,1,0);
  **(int **)(param_1 + 0xc) = (int)*psVar1;
  return;
}
