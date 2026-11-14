/* func_0890F4AC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_0890F544(int param_1,int param_2,char param_3);
extern int func_0891705C(int param_1);

void func_0890F4AC(int param_1)

{
  int iVar1;
  unsigned int *puVar2;
  int iVar3;
  
  iVar3 = 0;
  iVar1 = func_0891705C(0xffffffff);
  while (iVar3 = func_0890F544(param_1,iVar3,0), iVar3 != 0) {
    puVar2 = (unsigned int *)(iVar1 + ((unsigned int)(int)*(short *)(iVar3 + 0xc) >> 5) * 4 + 0x130);
    *puVar2 = *puVar2 & ~(1 << ((int)*(short *)(iVar3 + 0xc) & 0x1fU));
  }
  return;
}
