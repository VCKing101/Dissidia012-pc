/* func_088A9684: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088AA248(int param_1);

void func_088A9684(int *param_1)

{
  int iVar1;
  
  iVar1 = 0;
  while( 1 ) {
    if (iVar1 == 0) {
      iVar1 = *param_1;
    }
    else {
      iVar1 = *(int *)(iVar1 + 0x34);
    }
    if (iVar1 == 0) break;
    func_088AA248(iVar1);
  }
  return;
}
