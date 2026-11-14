/* func_088C7F9C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_088C7F78();

int func_088C7F9C(unsigned int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = -1;
  iVar1 = func_088C7F78();
  if (iVar1 == 1) {
    iVar2 = (int)(param_1 & 0x6000) >> 0xd;
  }
  return iVar2;
}
