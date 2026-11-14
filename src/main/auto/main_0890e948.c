/* func_0890E948: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08906D64();
extern void func_0890EA18();
extern int D_089D8364;
extern int D_09B6B3E0;

void func_0890E948(void)

{
  int *puVar1;
  int iVar2;
  
  if (D_089D8364 != 0) {
    func_08906D64();
    D_089D8364 = 0;
    iVar2 = 0;
    puVar1 = &D_09B6B3E0;
    do {
      func_0890EA18(*puVar1);
      *puVar1 = 0xffffffff;
      iVar2 = iVar2 + 1;
      puVar1 = puVar1 + 1;
    } while (iVar2 < 4);
  }
  return;
}
