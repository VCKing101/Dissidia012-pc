extern int func_0885C6CC();
extern int func_08865184();
extern int func_088D0B2C();
extern int func_08A4C598();
extern int func_08A4D738();
extern int func_08A4EDC0();
extern int func_08AF2CB0();
extern int func_08AF2D08();
extern int D_089D7830;
extern int D_08B01CF8;

void func_08A4E4E0(int p1,int p2)

{
  int uVar1;
  int param_1 = p1;
  int param_2 = p2;
  
  func_08A4C598();
  func_08A4EDC0(param_1);
  uVar1 = func_0885C6CC(*(int *)(param_1 + 0x3c),0xc8000,0x40,0,1,0);
  *(int *)(*(int *)(param_1 + 0x5c) + 0x6ef8) = uVar1;
  func_088D0B2C(D_089D7830,0x1a);
  func_08AF2CB0(D_08B01CF8);
  func_08AF2D08(D_08B01CF8,param_2,0,func_08A4D738,
               *(int *)(*(int *)(param_1 + 0x5c) + 0x6ef8),1,1,1,0,0xffffffff,0,0,0,0,0);
  func_08865184(param_1 + 4);
  return;
}
