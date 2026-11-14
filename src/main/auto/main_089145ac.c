/* func_089145AC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08864DB8(void);
extern int func_088AC9A8(int param_1);
extern void func_08916F38(void);
extern void func_08916F78(int param_1,int param_2);
extern void func_08917A9C(void);
extern int D_089D712C;

void func_089145AC(int param_1)

{
  int iVar1;
  
  iVar1 = func_088AC9A8(D_089D712C);
  if ((iVar1 != 0) && (60.0f <= *(float *)(param_1 + 0x6c))) {
    func_08916F78(0,func_08917A9C);
    func_08864DB8();
    func_08916F38();
  }
  return;
}
