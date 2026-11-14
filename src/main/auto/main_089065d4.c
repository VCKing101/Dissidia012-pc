/* func_089065D4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int * func_08905C58(int *param_1);

void func_089065D4(int *param_1)

{
  int *puVar1;
  int *puVar2;
  
  puVar2 = param_1 + 1;
  puVar1 = (int *)0x0;
  if (puVar2 != (int *)0x0) {
    func_08905C58(puVar2);
    puVar1 = puVar2;
  }
  param_1[0xb4] = puVar1;
  *param_1 = 0;
  return;
}
