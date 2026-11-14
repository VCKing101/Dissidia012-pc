/* func_08930098: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088D3F00(int param_1,int param_2,short *param_3);
extern void func_088D44EC(int param_1,int param_2,unsigned short *param_3);
extern int D_089CF138;
extern int D_089CF148;

void func_08930098(int param_1)

{
  func_088D3F00(0,param_1 + 0x30,&D_089CF138);
  func_088D44EC(param_1 + 0x30,param_1 + 0x34,&D_089CF148);
  return;
}
