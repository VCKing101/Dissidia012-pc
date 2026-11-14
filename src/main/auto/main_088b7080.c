/* func_088B7080: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_08934944();
extern int D_088B7054;

int func_088B7080(int *param_1,int param_2)

{
  
  if (param_2 == 0) {
    return 0;
  }
  else {
    return func_08934944(param_2,*param_1 + 8,*(int *)(*param_1 + 4),0x90,&D_088B7054);
  }
}
