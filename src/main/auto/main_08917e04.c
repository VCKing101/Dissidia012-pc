/* func_08917E04: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_0886503C(int param_1,int param_2,int param_3,int param_4);
extern void func_0886504C(int *param_1,int param_2,int param_3);
extern void func_08917D64(void);
extern int D_08917DA0;

void func_08917E04(int param_1,int param_2)

{
  func_0886503C(param_1 + 0x100,func_08917D64,&D_08917DA0,0);
  func_0886504C(param_1 + 0x100,param_1,0x100);
  *(int *)(param_1 + 0x128) = 0xffffffff;
  *(int *)(param_1 + 300) = 0xffffffff;
  *(int *)(param_1 + 0x134) = param_2;
  return;
}
