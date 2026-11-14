/* func_088AA248: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088A71D8(int param_1);

void func_088AA248(int param_1)

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
    func_088A71D8(iVar1);
  }
  return;
}
