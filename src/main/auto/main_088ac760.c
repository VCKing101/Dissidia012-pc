/* func_088AC760: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088ADF68(int param_1,int *param_2,int param_3,int param_4);
extern char * func_08935BB4(char *param_1,char *param_2,int param_3);
extern int D_089D6010;

void func_088AC760(int param_1)

{
  func_08935BB4(param_1 + 0x21c,D_089D6010,0xd);
  func_088ADF68(param_1,param_1 + 0x764,*(int *)(param_1 + 0xecc),
               *(int *)(param_1 + 0xec8));
  *(int *)(param_1 + 600) = 0x3ecc8;
  *(int *)(param_1 + 0x25c) = 0x3ecc8;
  *(int *)(param_1 + 0x254) = param_1 + 0xf08;
  *(int *)(param_1 + 0xef4) = 0;
  return;
}
