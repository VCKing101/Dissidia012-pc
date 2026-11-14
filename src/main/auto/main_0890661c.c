/* func_0890661C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int * func_0890631C(int *param_1,char *param_2);

void func_0890661C(int *param_1,int param_2)

{
  int *puVar1;
  int *puVar2;
  
  puVar2 = param_1 + 1;
  puVar1 = (int *)0x0;
  if (puVar2 != (int *)0x0) {
    func_0890631C(puVar2,param_2);
    puVar1 = puVar2;
  }
  param_1[0xb4] = puVar1;
  *param_1 = 0;
  return;
}
