/* func_089136DC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */

void func_089136DC(int param_1)

{
  int iVar1;
  
  iVar1 = 0;
  do {
    *(int *)(param_1 + 0x74) = 0;
    iVar1 = iVar1 + 1;
    param_1 = param_1 + 4;
  } while (iVar1 < 2);
  return;
}
