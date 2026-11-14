/* func_0892E7EC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D3F00(int param_1,int param_2,short *param_3);
extern void func_088D44EC(int param_1,int param_2,unsigned short *param_3);
extern void func_0892E8FC(int param_1);
extern int D_089CEFF0;
extern int D_089CF000;

void func_0892E7EC(int *param_1)

{
  if (*param_1 == 1) {
    func_088D3F00(0,param_1 + 3,&D_089CEFF0);
    func_088D44EC(param_1 + 3,param_1 + 4,&D_089CF000);
    func_0892E8FC(param_1);
  }
  return;
}
