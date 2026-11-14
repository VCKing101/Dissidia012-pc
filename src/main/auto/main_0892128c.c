/* func_0892128C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_089212F4(int param_1);
extern int func_08925378(int param_1);

void func_0892128C(int param_1,unsigned int param_2,unsigned char param_3)

{
  unsigned int uVar1;
  
  *(unsigned int *)(*(int *)(param_1 + 0xc) + 0x10) = *(unsigned int *)(*(int *)(param_1 + 0xc) + 0xc);
  *(unsigned int *)(*(int *)(param_1 + 0xc) + 0xc) = param_2;
  uVar1 = func_08925378(*(unsigned int *)(*(int *)(param_1 + 0xc) + 0xc));
  *(unsigned int *)(param_1 + 4) = uVar1;
  if (param_3 != '\0') {
    func_089212F4(param_1);
  }
  return;
}
