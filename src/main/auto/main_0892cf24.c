/* func_0892CF24: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D3F00(int param_1,int param_2,short *param_3);
extern void func_0892C7C8(int *param_1);
extern int D_089CED98;

void func_0892CF24(int param_1)

{
  int iVar1;
  
  if (*(int *)(param_1 + 0x28) == 1) {
    func_088D3F00(0,param_1 + 0x30,&D_089CED98);
    iVar1 = 0;
    param_1 = param_1 + 0x38;
    do {
      func_0892C7C8(param_1);
      iVar1 = iVar1 + 1;
      param_1 = param_1 + 0x28;
    } while (iVar1 < 2);
  }
  return;
}
