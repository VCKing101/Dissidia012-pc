/* func_08947100: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern unsigned int func_08944F7C(unsigned int param_1,int param_2);
extern void func_0894678C(float *param_1);
extern void func_0896FBC0(void);
extern void func_089A0BE4(int param_1,int param_2);

void func_08947100(int param_1)

{
  int iVar1;
  int iVar2;
  
  for (iVar2 = *(int *)(param_1 + 0x68); iVar2 != 0; iVar2 = *(int *)(iVar2 + 0x154)) {
    iVar1 = func_08944F7C(param_1,iVar2);
    if (iVar1 != 0) {
      func_0896FBC0();
    }
    if (*(unsigned char *)(iVar2 + 1) == '\x01') {
      func_089A0BE4((int)*(float *)(iVar2 + 0x194),(int)*(float *)(iVar2 + 0x198));
    }
    else {
      func_089A0BE4(0x710,0x778);
    }
    func_0894678C(iVar2 + 0x50);
  }
  return;
}
