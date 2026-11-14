/* func_08932AF4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0892AC2C();
extern void func_08931EB8();
extern void func_089323EC();

void func_08932AF4(int param_1)

{
  int iVar1;
  int iVar2;
  
  func_0892AC2C();
  *(int *)(param_1 + 0x28) = 0;
  iVar2 = 0;
  iVar1 = param_1;
  do {
    *(int *)(iVar1 + 0x2c) = 0;
    iVar2 = iVar2 + 1;
    iVar1 = iVar1 + 4;
  } while (iVar2 < 2);
  iVar2 = 0;
  iVar1 = param_1;
  do {
    *(int *)(iVar1 + 0x34) = 0;
    iVar2 = iVar2 + 1;
    iVar1 = iVar1 + 4;
  } while (iVar2 < 1);
  func_08931EB8(param_1 + 0x38);
  func_089323EC(param_1 + 0x5c);
  return;
}
