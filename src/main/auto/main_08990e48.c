/* func_08990E48: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern int func_089A5144(int param_1,int param_2);
extern int D_089DA420;

void func_08990E48(int param_1,int param_2)

{
  func_089A5144(param_1,(int)((float)param_2 *
                            (*(float *)(D_089DA420 + param_1 * 0x3c + 0x1ac) / 44100.0f)));
  return;
}
