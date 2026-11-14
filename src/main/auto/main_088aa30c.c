/* func_088AA30C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

unsigned int func_088AA30C(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = 0;
  while( 1 ) {
    if (iVar1 == 0) {
      iVar1 = *(int *)(param_1 + 0x1c);
    }
    else {
      iVar1 = *(int *)(iVar1 + 0x27c);
    }
    if (iVar1 == 0) break;
    if (*(int *)(iVar1 + 0xd0) == param_2) {
      return 1;
    }
  }
  return 0;
}
