/* func_088B3DFC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088B3EDC(int param_1,short *param_2,int param_3,int param_4,int param_5);
extern int func_088B5254(int param_1,int param_2,int param_3,int param_4,int param_5,
                int param_6);
extern void func_088B5670(int param_1,int param_2,int param_3,int param_4,int param_5,
                 int param_6,int param_7,int param_8);
extern void func_088B74A4(int param_1);

void func_088B3DFC(int param_1,int param_2,int param_3,int param_4,
                 int param_5,int param_6)

{
  int uVar1;
  int iVar2;
  char auStack_660 [1600];
  
  iVar2 = param_2 + *(char *)(param_2 + 0x763) * 0xc6;
  func_088B74A4(iVar2);
  func_088B3EDC(param_1,param_2,param_3,param_4,param_6);
  uVar1 = func_088B5254(param_1,auStack_660,200,param_2,param_3,param_5);
  func_088B5670(param_1,iVar2,auStack_660,uVar1,param_2,param_3,param_5,param_4);
  return;
}
