/* func_0888B804: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08865158(int param_1);
extern void func_0888964C(int param_1,unsigned int param_2,int param_3,unsigned int param_4,unsigned char param_5);
extern void func_08889808(int param_1);
extern int D_089D6A10;

void func_0888B804(int param_1)

{
  func_08865158(param_1 + 0x100);
  func_0888964C(D_089D6A10,*(unsigned int *)(D_089D6A10 + 0x4c),
               *(unsigned int *)(D_089D6A10 + 0x54),*(unsigned int *)(D_089D6A10 + 0x740),
               *(unsigned char *)(param_1 + 0x134));
  func_08889808(D_089D6A10);
  return;
}
