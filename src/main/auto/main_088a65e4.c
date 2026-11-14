/* func_088A65E4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_089418F8();

int func_088A65E4(int *param_1)

{
  int iVar1;
  
  iVar1 = param_1[2];
  if (*param_1 != -1) {
    func_089418F8(param_1[1],*param_1);
    *param_1 = -1;
    param_1[2] = 0;
  }
  return iVar1;
}
