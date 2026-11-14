/* func_0892B2C0: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D44EC(unsigned int param_1,int param_2,unsigned short *param_3);
extern void func_0892B7F8(unsigned int param_1,int param_2,int param_3,unsigned int param_4);
extern int D_089CECA0;

void func_0892B2C0(int param_1)

{
  func_0892B7F8(param_1,*(unsigned int *)(param_1 + 0x34),*(unsigned int *)(param_1 + 0x14),
               *(unsigned short *)(param_1 + 0x40));
  func_0892B7F8(param_1,*(unsigned int *)(param_1 + 0x34),*(unsigned int *)(param_1 + 0x18),
               *(unsigned short *)(param_1 + 0x3a));
  func_0892B7F8(param_1,*(unsigned int *)(param_1 + 0x34),*(unsigned int *)(param_1 + 0x1c),
               *(unsigned short *)(param_1 + 0x3c));
  func_0892B7F8(param_1,*(unsigned int *)(param_1 + 0x34),*(unsigned int *)(param_1 + 0x20),
               *(unsigned short *)(param_1 + 0x3e));
  func_0892B7F8(param_1,*(unsigned int *)(param_1 + 0x34),*(unsigned int *)(param_1 + 0x10),
               *(unsigned short *)(param_1 + 0x38));
  func_0892B7F8(param_1,*(unsigned int *)(param_1 + 0x10),*(unsigned int *)(param_1 + 0xc),0);
  func_0892B7F8(param_1,*(unsigned int *)(param_1 + 0x10),*(unsigned int *)(param_1 + 8),1);
  func_088D44EC(param_1 + 8,param_1 + 0x24,&D_089CECA0);
  return;
}
