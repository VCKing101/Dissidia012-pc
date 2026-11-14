/* func_08956C64: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_08971010();

void func_08956C64(int param_1,int param_2,int param_3,int *param_4)

{
  int iVar1;
  
  if (*param_4 != 0) {
    iVar1 = func_08971010(*(int *)(param_2 + 0x18),*param_4);
    param_2 = param_2 + (unsigned int)*(unsigned short *)(*(int *)(*(int *)(param_2 + 0x88) + 0x1c8) + 0x18) * 4;
    *(int *)(param_2 + 0xb4) = *(int *)(param_2 + 0xb4) - iVar1;
  }
  return;
}
