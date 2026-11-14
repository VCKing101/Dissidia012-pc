/* func_08930D24: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D3CF0(int param_1,int param_2,unsigned short *param_3);
extern int func_088DAA68(int param_1);
extern int D_089CF1CC;

void func_08930D24(int param_1)

{
  int uVar1;
  
  uVar1 = func_088DAA68(*(int *)(param_1 + 0x30));
  *(int *)(param_1 + 0x34) = uVar1;
  func_088D3CF0(0,param_1 + 0x34,&D_089CF1CC);
  *(short *)(*(int *)(param_1 + 0x34) + 0xe) = 0x14;
  *(int *)(param_1 + 0x28) = 1;
  return;
}
