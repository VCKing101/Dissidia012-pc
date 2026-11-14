/* func_0891E18C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0886503C(int param_1,int param_2,int param_3,int param_4);
extern void func_0886504C(int *param_1,int param_2,int param_3);
extern void func_088650A4(int param_1);
extern void func_0891E2A0(void);
extern void func_0891E2C0(void);

void func_0891E18C(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 + 0x100;
  func_0886503C(iVar1,func_0891E2A0,func_0891E2C0,0);
  func_0886504C(iVar1,param_1,0x100);
  func_088650A4(iVar1);
  return;
}
