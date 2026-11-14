/* func_088AC2F8: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088AC294(int param_1,int param_2);
extern void func_088ADF68(int param_1,int *param_2,int param_3,int param_4);
extern char * func_08935BB4(char *param_1,char *param_2,int param_3);

void func_088AC2F8(int param_1,int param_2,int param_3,int param_4,
                 int param_5)

{
  func_08935BB4(param_1 + 0x21c,param_4,0xd);
  func_088ADF68(param_1,param_1 + 0x764,0,0);
  *(int *)(param_1 + 600) = param_3;
  *(int *)(param_1 + 0x25c) = param_3;
  *(int *)(param_1 + 0x254) = param_2;
  *(int *)(param_1 + 0xef4) = param_5;
  func_088AC294(param_1,4);
  return;
}
